#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif // !WIN32_LEAN_AND_MEAN

#include <stdio.h>
#include <string>
#include <iostream>

//Kinect head file
#include <Kinect.h>

//Safe release for interfaces.
template <class Interface>
inline void SafeRelease(Interface *& pInterfaceToRelease)
{
	if (pInterfaceToRelease != nullptr)
	{
		pInterfaceToRelease->Release();
		pInterfaceToRelease = nullptr;
	}
}

using namespace std;

class CKinect_API
{
	//Size of image
	static const int cWidth = 512;
	static const int cHeight = 424;

public:
	//Constructor
	CKinect_API();

	//Distructor
	~CKinect_API();

private:

	//Kinect sensor
	IKinectSensor * m_pKinectSensor;

	//Main frame data reader.
	IMultiSourceFrameReader* m_pMultiSourceReader;

	//Reader for different sources.
	IDepthFrameReader* m_pDepthReader;
	IColorFrameReader* m_pColorReader;
	IInfraredFrameReader* m_pInfraredReader;


public:
	//Camera Space Points
	CameraSpacePoint * m_pCameraSpacePoint;

public:
	//Initialize the defualt sensor
	HRESULT InitializeSensor();

	//Handle new depth data
	//	pBuffer: Pointer to depth frame data.
	//	nHeight: Height of depth frame.
	//	nWidth:	Width of depth frame.
	//	nMinDepth: The minimum reliable depth of data.
	//	nMaxDepth: The maxmum reliable depth of data.
	void ProcessDepth(UINT16* pBuffer, int nHeight, int nWidth, USHORT nMinDepth, USHORT nMaxDepth);

	//Get the main frame reader.
	HRESULT GetMultiSuorceFrame();

	//Gets depth frame data.
	void GetDepthFrame();

	//Gets color frame data.
	void GetColorFrame();

	//Gets infrared frame data.
	void GetInfraredFrame();

	//Gets properties of image.
	int GetWidth();
	int GetHeight();

};