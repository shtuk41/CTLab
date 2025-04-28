
#include <STEPControl_Reader.hxx>
#include <BRepMesh_IncrementalMesh.hxx>

#include <BRep_Tool.hxx>
#include <TopoDS.hxx>
#include <TopoDS_Face.hxx>
#include <TopExp_Explorer.hxx>
#include <Poly_Triangulation.hxx>

#include <glm/glm.hpp>

#include <stp_parser.h>

//#define OUTPUT_TO_FILE

#ifdef OUTPUT_TO_FILE
#include <fstream>
#endif

std::vector<triangle> StpParser::ParseToTriangles(const char* stlFileName, unsigned int& number_of_triangles, double meshGenLinearDeflection)
{
    std::vector<triangle> triangles;
    number_of_triangles = 0;

#ifdef OUTPUT_TO_FILE
    std::ofstream file("stp_parser.txt");
#endif

    try 
    {
        STEPControl_Reader reader;
        std::string filePath = std::string(stlFileName);
        reader.ReadFile(filePath.c_str());

#ifdef OUTPUT_TO_FILE
        file << "STEP File loaded" << std::endl;
        file << "Linear Deflection: " << meshGenLinearDeflection << std::endl;
#endif

        reader.TransferRoots();
        auto theShape = reader.OneShape();

        BRepMesh_IncrementalMesh stpMesh(theShape, meshGenLinearDeflection);
        theShape = stpMesh.Shape();

        Standard_Integer aNbNodes = 0;
        Standard_Integer aNbTriangles = 0;

        // calculate total number of the nodes and triangles
        for (TopExp_Explorer anExpSF(theShape, TopAbs_FACE); anExpSF.More(); anExpSF.Next())
        {
            TopLoc_Location aLoc;
            Handle(Poly_Triangulation) aTriangulation = BRep_Tool::Triangulation(TopoDS::Face(anExpSF.Current()), aLoc);
            if (!aTriangulation.IsNull())
            {
                aNbNodes += aTriangulation->NbNodes();
                aNbTriangles += aTriangulation->NbTriangles();
            }
        }

        if (aNbTriangles == 0)
        {
            throw Standard_Failure("no triangulation in shape");
        }
        else
        {
            number_of_triangles = aNbTriangles;
        }

#ifdef OUTPUT_TO_FILE
        file << "Number of triangles: " << aNbTriangles << std::endl;
#endif

        triangle  next;

        // create temporary triangulation
        Handle(Poly_Triangulation) aMesh = new Poly_Triangulation(aNbNodes, aNbTriangles, Standard_False);
        // count faces missing triangulation
        Standard_Integer aNbFacesNoTri = 0;
        // fill temporary triangulation
        Standard_Integer aNodeOffset = 0;
        Standard_Integer aTriangleOffet = 0;
        for (TopExp_Explorer anExpSF(theShape, TopAbs_FACE); anExpSF.More(); anExpSF.Next())
        {
            const TopoDS_Shape& aFace = anExpSF.Current();
            TopLoc_Location aLoc;
            Handle(Poly_Triangulation) aTriangulation = BRep_Tool::Triangulation(TopoDS::Face(aFace), aLoc);
            if (aTriangulation.IsNull())
            {
                ++aNbFacesNoTri;
                continue;
            }

            // copy nodes
            gp_Trsf aTrsf = aLoc.Transformation();
            for (Standard_Integer aNodeIter = 1; aNodeIter <= aTriangulation->NbNodes(); ++aNodeIter)
            {
                gp_Pnt aPnt = aTriangulation->Node(aNodeIter);
                aPnt.Transform(aTrsf);
                aMesh->SetNode(aNodeIter + aNodeOffset, aPnt);
            }

            // copy triangles
            const TopAbs_Orientation anOrientation = anExpSF.Current().Orientation();
            for (Standard_Integer aTriIter = 1; aTriIter <= aTriangulation->NbTriangles(); ++aTriIter)
            {
                Poly_Triangle aTri = aTriangulation->Triangle(aTriIter);

                Standard_Integer anId[3];
                aTri.Get(anId[0], anId[1], anId[2]);
                if (anOrientation == TopAbs_REVERSED)
                {
                    // Swap 1, 2.
                    Standard_Integer aTmpIdx = anId[1];
                    anId[1] = anId[2];
                    anId[2] = aTmpIdx;
                }

                // Update nodes according to the offset.
                anId[0] += aNodeOffset;
                anId[1] += aNodeOffset;
                anId[2] += aNodeOffset;

                int outputIndex = (aTriIter + aTriangleOffet - 1) * 3;

                next.points[0].data[0] = (float)(aMesh->Node(anId[0]).X());
                next.points[0].data[1] = (float)(aMesh->Node(anId[0]).Y());
                next.points[0].data[2] = (float)(aMesh->Node(anId[0]).Z());

                next.points[1].data[0] = (float)(aMesh->Node(anId[1]).X());
                next.points[1].data[1] = (float)(aMesh->Node(anId[1]).Y());
                next.points[1].data[2] = (float)(aMesh->Node(anId[1]).Z());
                  
                next.points[2].data[0] = (float)(aMesh->Node(anId[2]).X());
                next.points[2].data[1] = (float)(aMesh->Node(anId[2]).Y());
                next.points[2].data[2] = (float)(aMesh->Node(anId[2]).Z());

                //calculate normal

                gp_Vec edge1(aMesh->Node(anId[1]).XYZ() - (aMesh->Node(anId[0]).XYZ()));
                gp_Vec edge2(aMesh->Node(anId[2]).XYZ() - (aMesh->Node(anId[0]).XYZ()));

                // Compute cross product to get normal vector
                gp_Vec normal = edge1.Crossed(edge2);

                // Normalize normal vector
                normal.Normalize();

                next.norm.data[0] = (float)normal.X();
                next.norm.data[1] = (float)normal.Y();
                next.norm.data[2] = (float)normal.Z();

                triangles.push_back(next);


#ifdef OUTPUT_TO_FILE
                for (int ii = 0; ii < 3; ii++)
                {
                    file << next.points[ii].data[0] << ',';
                    file << next.points[ii].data[1] << ',';
                    file << next.points[ii].data[2] << ',';
                }

                file << std::endl;
#endif

                aTri.Set(anId[0], anId[1], anId[2]);
                aMesh->SetTriangle(aTriIter + aTriangleOffet, aTri);
            }

            aNodeOffset += aTriangulation->NbNodes();
            aTriangleOffet += aTriangulation->NbTriangles();
        }

        return triangles;

    }
    catch (Standard_Failure) 
    {
#ifdef OUTPUT_TO_FILE
        file << "Error: Failed to create mesh from file." << std::endl;
#endif
        std::cerr << "Error: Failed to create mesh from STP file." << std::endl;
        triangles.clear();
        number_of_triangles = 0;
    }

	return triangles;
}