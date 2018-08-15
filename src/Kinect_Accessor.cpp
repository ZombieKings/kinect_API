#include "Kinect_Accessor.h"

//Constructor
CKinect_Accessor::CKinect_Accessor()
{
	m_pKinectSensor = nullptr;
	m_pDepthReader = nullptr;
	m_pCameraSpacePoint = nullptr;
}

//Distructor
CKinect_Accessor::~CKinect_Accessor()
{
	//clean points data
	delete[] m_pCameraSpacePoint;
	m_pCameraSpacePoint = nullptr;

	//clean up depth frame reader
	SafeRelease(m_pDepthReader);

	//close the Kinect Sensor
	if (m_pKinectSensor)
	{
		m_pKinectSensor->Close();
	}
	SafeRelease(m_pKinectSensor);

}

//Initialize the defualt sensor.
HRESULT CKinect_Accessor::InitializeSensor()
{
	HRESULT hr;

	hr = GetDefaultKinectSensor(&m_pKinectSensor);

	if (FAILED(hr))
	{
		return hr;
	}

	if (m_pKinectSensor)
	{
		//Initialize the Kinect and get the depth reader.
		IDepthFrameSource* pDepthFrameSource = nullptr;

		hr = m_pKinectSensor->Open();

		if (SUCCEEDED(hr))
		{
			hr = m_pKinectSensor->get_DepthFrameSource(&pDepthFrameSource);
		}

		if (SUCCEEDED(hr))
		{
			hr = pDepthFrameSource->OpenReader(&m_pDepthReader);
		}
		SafeRelease(pDepthFrameSource);
	}

	if (!m_pKinectSensor || FAILED(hr))
	{
		cout << "No kinect sensor device detected." << endl;
		return E_FAIL;
	}

	return hr;
}

//Handle new depth data
//	pBuffer: Pointer to depth frame data.
//	nHeight: Height of depth frame.
//	nWidth:	Width of depth frame.
//	nMinDepth: The minimum reliable depth of data.
//	nMaxDepth: The maxmum reliable depth of data.
void CKinect_Accessor::ProcessDepth(UINT16* pBuffer, int nWidth, int nHeight, USHORT nMinDepth, USHORT nMaxDepth)
{
	//check out the validity of data
	if (pBuffer && (nWidth == cWidth) && (nHeight == cHeight))
	{
		//create a mapper
		ICoordinateMapper* pICoordinateMapper;

		HRESULT hr = m_pKinectSensor->get_CoordinateMapper(&pICoordinateMapper);

		if (SUCCEEDED(hr))
		{
			m_pCameraSpacePoint = new CameraSpacePoint[nHeight*nWidth];
			pICoordinateMapper->MapDepthFrameToCameraSpace(nHeight*nWidth, pBuffer, nHeight*nWidth, m_pCameraSpacePoint);
		}

		SafeRelease(pICoordinateMapper);
	}
}

// Get the latest depth frame data
void CKinect_Accessor::Update()
{

	if (!m_pDepthReader)
	{
		return;
	}

	IDepthFrame* pDepthFrame = nullptr;
	HRESULT hr = m_pDepthReader->AcquireLatestFrame(&pDepthFrame);

	while (!SUCCEEDED(hr))
	{
		hr = m_pDepthReader->AcquireLatestFrame(&pDepthFrame);
	}

	if (SUCCEEDED(hr))
	{
		IFrameDescription* pFrameDescription = nullptr;
		int nWidth = 0;
		int nHeight = 0;
		USHORT nDepthMinReliableDistance = 0;
		USHORT nDepthMaxReliableDistance = 0;
		UINT nBufferSize = 0;
		UINT16 *pBuffer = nullptr;

		if (SUCCEEDED(hr))
		{
			hr = pDepthFrame->get_FrameDescription(&pFrameDescription);
		}

		if (SUCCEEDED(hr))
		{
			hr = pFrameDescription->get_Width(&nWidth);
		}

		if (SUCCEEDED(hr))
		{
			hr = pFrameDescription->get_Height(&nHeight);
		}

		if (SUCCEEDED(hr))
		{
			hr = pDepthFrame->get_DepthMinReliableDistance(&nDepthMinReliableDistance);
		}

		if (SUCCEEDED(hr))
		{
			hr = pDepthFrame->get_DepthMaxReliableDistance(&nDepthMaxReliableDistance);
		}

		if (SUCCEEDED(hr))
		{
			hr = pDepthFrame->AccessUnderlyingBuffer(&nBufferSize, &pBuffer);
		}

		if (SUCCEEDED(hr))
		{
			ProcessDepth(pBuffer, nWidth, nHeight, nDepthMinReliableDistance, nDepthMaxReliableDistance);
		}

		SafeRelease(pFrameDescription);
	}

	SafeRelease(pDepthFrame);


}

int CKinect_Accessor::GetWidth()
{
	return cWidth;
}
int CKinect_Accessor::GetHeight()
{
	return cHeight;
}

