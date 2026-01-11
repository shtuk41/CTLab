#include <context.h>

Context::Context()
{

}

Context::Context(const std::string& path)
{
	setVolume(path);
}

void Context::initGL()
{
	if (initialized)
		return;

	qDebug() << "Context started initialization";

	QOpenGLFunctions_3_3_Core f;
	f.initializeOpenGLFunctions();

	f.glGenTextures(1, &tex3D);
	f.glBindTexture(GL_TEXTURE_3D, tex3D);
	f.glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	f.glTexImage3D(GL_TEXTURE_3D, 0, GL_R8, width, height, depth, 0,
		GL_RED, GL_UNSIGNED_BYTE, volumeData->getVolumeDataTex().data());

	f.glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	f.glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	f.glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	f.glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	f.glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	initialized = true;
}

void Context::setXDistance(float xD)
{
	xDistance = xD;

	if (onDistanceChanged)
	{
		onDistanceChanged();
	}
}

void Context::setYDistance(float yD)
{
	yDistance = yD;

	if (onDistanceChanged)
	{
		onDistanceChanged();
	}
}

void Context::setZDistance(float zD)
{
	zDistance = zD;

	if (onDistanceChanged)
	{
		onDistanceChanged();
	}
}

bool Context::setVolume(const std::string& path)
{
	volumeData = std::make_unique<VolumeData>(path);
	width = volumeData->getHeader()->recoX;
	height = volumeData->getHeader()->recoY;
	depth = volumeData->getHeader()->recoZ;
	volumeData->saveHeaderToFile("volumeHeader.txt");
	volumeData->fillBuffer();

	return false;
}

