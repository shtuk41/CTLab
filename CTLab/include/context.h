#pragma once

#include <numbers>
#include <vector>

#include <camera.h>
#include <GLFW/glfw3.h>

struct Context
{
	//infrmation
	float rotateY;
	float rotateX;
	View view;
	std::unique_ptr<Camera> cameraSensor;
	float zOffset = 0.0f;
	float previousZOffset = 0.0f;
	float latShift = 0.0f;
	float vertShift = 0.0f;

	//screenshot saving
	char screenShotName[256];
	char screenShotScalePercent[4];
	bool screenshotSaveToSize = false;
	char saveWidth[5];
	char saveHeight[5];
	int listbox_item_current_last;
	std::vector<std::unique_ptr<char[]>> fov_items;
	bool startSavingAll;
	char outputDirectoryPath[256];
	char overlayViewPercent[4];

	//projection settings
	float perspectiveFovyRadians = std::numbers::pi_v<float> / 2;
	bool usePerspectiveProjection = true;
	char orthoLeft[5];
	char orthoRight[5];
	char orthoBottom[5];
	char orthoTop[5];
	char orthoNear[5];
	char orthoFar[5];

	//overlay settings
	bool useOverlays = false;
	char pathToOverlays[256] = ".\\Acquired";
	char overlayImageName[50] = "ASIS_Raw01.png";

	//mask display flag
	bool showMask = true;

	//surface/edge adapting flag
	bool useSurfaceEdgeAdapting = true;
	bool adaptSurfaceEdgeSingleShot = false;

	//normals
	bool drawAllNormals = false;


	Context(GLFWwindow* window);
	void SaveImage();
	void ReadPathPlan();
	int sync_current_fov_number();
	bool updateListBoxCurrentLast(int fov_selected);
	void saveAllScreenshotsBW(float percent_scale);
	float GetScreenshotScalePercent();
	void SetScreenshotScalePercent(const std::string &percent);
	void SetUsePerspectiveProjection(const std::string& use_perspective_projection);
	void SetPerspetiveAngle(const std::string& perspective_angle);
	void SetOutputDirectoryPath(const std::string& output_directory_path);
	float GetOverlayViewPercent();
	void SetOverlayViewPercent(const std::string& overlay_view_percent);


	void SetUseOverlays(const std::string& use_overlays);
	void SetPathToOverlays(const std::string& path_overlays);
	void SetOverlayImageName(const std::string& overlay_image_name);

	int GetSaveWidth();
	int GetSaveHeight();
	float GetOrthoLeft();
	float GetOrthoRight();
	float GetOrthoBottom();
	float GetOrthoTop();
	float GetOrthoNear();
	float GetOrthoFar();
};

