
#include <vector>
#include <framebufferCapture.h>

static void resize(const cv::Mat& src, cv::Mat& dest, float scale_percent)
{
	int viewwidth = int(src.cols * scale_percent / 100);
	int viewheight = int(src.rows * scale_percent / 100);
	cv::Size viewDim = { viewwidth, viewheight };
	cv::resize(src, dest, viewDim, cv::INTER_AREA);
}

static void resizeToSize(const cv::Mat& src, cv::Mat& dest, int width, int height)
{
	cv::Size viewDim = { width, height };
	cv::resize(src, dest, viewDim, cv::INTER_AREA);
}

void FramebufferCapture::SaveFrameToImageColor(std::string filename)
{
	int width, height;
	glfwGetFramebufferSize(&(*window), &width, &height);
	GLsizei nrChannels = 3;
	GLsizei stride = nrChannels * width;
	stride += (stride % 4) ? (4 - stride % 4) : 0;
	GLsizei bufferSize = stride * height;
	std::vector<unsigned char> buffer(bufferSize);
	glPixelStorei(GL_PACK_ALIGNMENT, 4);
	glReadBuffer(GL_FRONT);
	glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, buffer.data());

	unsigned char* finalData = new unsigned char[buffer.size()];

	int vcount = height - 1;

	for (int ii = 0; ii < height; ii++)
	{
		memcpy(&finalData[vcount * stride], buffer.data() + ii * stride, stride * sizeof(unsigned char));
		vcount--;
	}

	/*SOIL_save_image
	(
		filename.c_str(),
		SOIL_SAVE_TYPE_BMP,
		width, height, nrChannels,
		(const unsigned char*)finalData
	);*/

	cv::Mat dest(height, width, CV_8UC3, finalData);
	cv::cvtColor(dest, dest, cv::COLOR_BGR2RGB);
	cv::imwrite(filename, dest);
	delete[] finalData;
}

void FramebufferCapture::SaveFrameToImageColorResized(std::string filename, float scale_percent)
{
	int width, height;
	glfwGetFramebufferSize(&(*window), &width, &height);
	GLsizei nrChannels = 3;
	GLsizei stride = nrChannels * width;
	stride += (stride % 4) ? (4 - stride % 4) : 0;
	GLsizei bufferSize = stride * height;
	std::vector<unsigned char> buffer(bufferSize);
	glPixelStorei(GL_PACK_ALIGNMENT, 4);
	glReadBuffer(GL_FRONT);
	glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, buffer.data());

	unsigned char* finalData = new unsigned char[buffer.size()];

	int vcount = height - 1;

	for (int ii = 0; ii < height; ii++)
	{
		memcpy(&finalData[vcount * stride], buffer.data() + ii * stride, stride * sizeof(unsigned char));
		vcount--;
	}

	cv::Mat dest(height, width, CV_8UC3, finalData);
	cv::cvtColor(dest, dest, cv::COLOR_BGR2RGB);
	resize(dest, dest, scale_percent);
	cv::imwrite(filename, dest);
	delete[] finalData;
}

void FramebufferCapture::SaveFrameToImageColorResizedToSize(std::string filename, int saveWidth, int saveHeight)
{
	int width, height;
	glfwGetFramebufferSize(&(*window), &width, &height);
	GLsizei nrChannels = 3;
	GLsizei stride = nrChannels * width;
	stride += (stride % 4) ? (4 - stride % 4) : 0;
	GLsizei bufferSize = stride * height;
	std::vector<unsigned char> buffer(bufferSize);
	glPixelStorei(GL_PACK_ALIGNMENT, 4);
	glReadBuffer(GL_FRONT);
	glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, buffer.data());

	unsigned char* finalData = new unsigned char[buffer.size()];

	int vcount = height - 1;

	for (int ii = 0; ii < height; ii++)
	{
		memcpy(&finalData[vcount * stride], buffer.data() + ii * stride, stride * sizeof(unsigned char));
		vcount--;
	}

	cv::Mat dest(height, width, CV_8UC3, finalData);
	cv::cvtColor(dest, dest, cv::COLOR_BGR2RGB);
	resizeToSize(dest, dest, saveWidth, saveHeight);
	cv::imwrite(filename, dest);
	delete[] finalData;
}


//https://lencerf.github.io/post/2019-09-21-save-the-opengl-rendering-to-image-file/
void FramebufferCapture::SaveFrameToImageBW(std::string filename)
{
	int width, height;
	glfwGetFramebufferSize(&(*window), &width, &height);
	GLsizei nrChannels = 1;
	GLsizei stride = nrChannels * width;
	stride += (stride % 4) ? (4 - stride % 4) : 0;
	GLsizei bufferSize = stride * height;
	std::vector<unsigned char> buffer(bufferSize);
	glPixelStorei(GL_PACK_ALIGNMENT, 4);
	glReadBuffer(GL_FRONT);
	glReadPixels(0, 0, width, height, GL_RED, GL_UNSIGNED_BYTE, buffer.data());

	unsigned char* finalData = new unsigned char[buffer.size()];

	int vcount = height - 1;

	for (int ii = 0; ii < height; ii++)
	{
		memcpy(&finalData[vcount * stride], buffer.data() + ii * stride, stride * sizeof(unsigned char));
		vcount--;
	}

	cv::Mat dest(height, width, CV_8UC1, finalData);
	cv::cvtColor(dest, dest, cv::COLOR_BGR2RGB);

	cv::imwrite(filename, dest);
	delete[] finalData;
}

//https://lencerf.github.io/post/2019-09-21-save-the-opengl-rendering-to-image-file/
void FramebufferCapture::SaveFrameToImageBWResized(std::string filename, float scale_percent)
{
	int width, height;
	glfwGetFramebufferSize(&(*window), &width, &height);
	GLsizei nrChannels = 1;
	GLsizei stride = nrChannels * width;
	stride += (stride % 4) ? (4 - stride % 4) : 0;
	GLsizei bufferSize = stride * height;
	std::vector<unsigned char> buffer(bufferSize);
	glPixelStorei(GL_PACK_ALIGNMENT, 4);
	glReadBuffer(GL_FRONT);
	glReadPixels(0, 0, width, height, GL_RED, GL_UNSIGNED_BYTE, buffer.data());

	unsigned char* finalData = new unsigned char[buffer.size()];

	int vcount = height - 1;

	for (int ii = 0; ii < height; ii++)
	{
		memcpy(&finalData[vcount * stride], buffer.data() + ii * stride, stride * sizeof(unsigned char));
		vcount--;
	}

	cv::Mat dest(height, width, CV_8UC1, finalData);
	cv::cvtColor(dest, dest, cv::COLOR_BGR2RGB);

	resize(dest, dest, scale_percent);

	cv::imwrite(filename, dest);
	delete[] finalData;
}

void FramebufferCapture::SaveFrameToImageBWResizedToSize(std::string filename, int saveWidth, int saveHeight)
{
	int width, height;
	glfwGetFramebufferSize(&(*window), &width, &height);
	GLsizei nrChannels = 1;
	GLsizei stride = nrChannels * width;
	stride += (stride % 4) ? (4 - stride % 4) : 0;
	GLsizei bufferSize = stride * height;
	std::vector<unsigned char> buffer(bufferSize);
	glPixelStorei(GL_PACK_ALIGNMENT, 4);
	glReadBuffer(GL_FRONT);
	glReadPixels(0, 0, width, height, GL_RED, GL_UNSIGNED_BYTE, buffer.data());

	unsigned char* finalData = new unsigned char[buffer.size()];

	int vcount = height - 1;

	for (int ii = 0; ii < height; ii++)
	{
		memcpy(&finalData[vcount * stride], buffer.data() + ii * stride, stride * sizeof(unsigned char));
		vcount--;
	}

	cv::Mat dest(height, width, CV_8UC1, finalData);
	cv::cvtColor(dest, dest, cv::COLOR_BGR2RGB);

	resizeToSize(dest, dest, saveWidth, saveHeight);

	cv::imwrite(filename, dest);
	delete[] finalData;
}

cv::Mat FramebufferCapture::GetCurrentFrameAsMat()
{
	int width, height;
	glfwGetFramebufferSize(&(*window), &width, &height);
	GLsizei nrChannels = 1;
	GLsizei stride = nrChannels * width;
	stride += (stride % 4) ? (4 - stride % 4) : 0;
	GLsizei bufferSize = stride * height;
	std::vector<unsigned char> buffer(bufferSize);
	glPixelStorei(GL_PACK_ALIGNMENT, 4);
	glReadBuffer(GL_FRONT);
	glReadPixels(0, 0, width, height, GL_RED, GL_UNSIGNED_BYTE, buffer.data());

	unsigned char* finalData = new unsigned char[buffer.size()];

	int vcount = height - 1;

	for (int ii = 0; ii < height; ii++)
	{
		memcpy(&finalData[vcount * stride], buffer.data() + ii * stride, stride * sizeof(unsigned char));
		vcount--;
	}

	cv::Mat dest(height, width, CV_8UC1, finalData);

	return dest;
}



