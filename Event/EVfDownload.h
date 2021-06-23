#pragma once

#include "Observer.h"
#include "ActionSource.h"
#include "CameraEvent.h"
#include "CameraController.h"


typedef void(CALLBACK *IMAGEDATACALLBACK)(CComPtr<IStream> stream,LPARAM lParam);
class CEVfDownload : public ActionSource , public Observer
{
public:
	CEVfDownload(void)
	{
		active = FALSE;
		memset(&m_focusInfo, 0, sizeof(EdsFocusInfo));
	}
	//~CEVFPicture(void);
public:
	virtual void update(Observable* from, CameraEvent *e)
	{

		std::string event = e->getEvent();

		if(event == "EvfDataChanged")
		{
			EVF_DATASET data = *static_cast<EVF_DATASET *>(e->getArg());

			//The update processing can be executed from another thread. 
			SaveImage(data,m_path);

			EdsInt32 propertyID = kEdsPropID_FocusInfo;
			fireEvent("get_Property", &propertyID);

			// Download image data.
			fireEvent("downloadEVF");
		}

		if (event == "PropertyChanged")
		{
			EdsInt32 proeprtyID = *static_cast<EdsInt32 *>(e->getArg());
			if(proeprtyID == kEdsPropID_Evf_OutputDevice)
			{
				CameraModel* model = (CameraModel *)from;
				EdsUInt32 device = model->getEvfOutputDevice();

				// PC live view has started.
				if (!active && (device & kEdsEvfOutputDevice_PC) != 0)
				{
					active = TRUE;
					// Start download of image data.
					fireEvent("downloadEVF");
				}

				// PC live view has ended.
				if(active && (device & kEdsEvfOutputDevice_PC) == 0)
				{
					active = FALSE;
				}
			}
			if(proeprtyID == kEdsPropID_FocusInfo)
			{
				//CameraModel* model = (CameraModel *)from;
				//float xRatio = 1;
				//float yRatio = 1;
				//CRect rect;
				//GetWindowRect(&rect);

				//EdsFocusInfo focusInfo = {0};
				//focusInfo = model->getFocusInfo();
				//m_focusInfo = focusInfo;
				//xRatio = (float)(rect.Width())/(float)(focusInfo.imageRect.size.width);
				//yRatio = (float)(rect.Height())/(float)(focusInfo.imageRect.size.height);
				//for(EdsUInt32 i = 0; i < focusInfo.pointNumber; i++)
				//{
				//	m_focusInfo.focusPoint[i].rect.point.x = (EdsUInt32)(m_focusInfo.focusPoint[i].rect.point.x * xRatio);
				//	m_focusInfo.focusPoint[i].rect.point.y = (EdsUInt32)(m_focusInfo.focusPoint[i].rect.point.y * yRatio);
				//	m_focusInfo.focusPoint[i].rect.size.width = (EdsUInt32)(m_focusInfo.focusPoint[i].rect.size.width * xRatio);
				//	m_focusInfo.focusPoint[i].rect.size.height = (EdsUInt32)(m_focusInfo.focusPoint[i].rect.size.height * yRatio);
				//}
			}
			if(proeprtyID==kEdsPropID_Evf_AFMode)
			{
				CameraModel* model = (CameraModel *)from;
				m_bDrawZoomFrame = model->getEvfAFMode()!=2;
			}
		}	
	}
private:
	BOOL active;
	EdsFocusInfo	m_focusInfo;
	EdsBool			m_bDrawZoomFrame;
	LPCTSTR			m_path;

public:	
	IMAGEDATACALLBACK _imagedata;
	LPARAM			  _lParam;
	void SetCallback(IMAGEDATACALLBACK imagedata,LPARAM lParam){_imagedata=imagedata;_lParam=lParam;};


	void SaveImage(EVF_DATASET data,LPCTSTR path)
	{
		EdsUInt64 size;

		unsigned char* pbyteImage = NULL; 

		// Get image (JPEG) pointer.
		EdsGetPointer(data.stream, (EdsVoid**)&pbyteImage );

		if (pbyteImage!=NULL)
		{
			EdsGetLength(data.stream, &size);


			CComPtr<IStream> stream;
			stream = NULL;

			HGLOBAL hMem = ::GlobalAlloc(GHND, size); //�Ӷ��з���һ����Ŀ���ֽ��� GHNDΪ���䷽ʽ�����ƶ��ҳ�ʼΪ0������һ���·�����ڴ����ľ����
			LPVOID pBuff = ::GlobalLock(hMem);//�����ڴ���ָ�����ڴ�飬������һ����ֵַ������ָ���ڴ�����ʼ���������� GlobalUnlock �������ڴ������������ַ��һֱ������Ч��

			memcpy(pBuff, pbyteImage, size);

			::GlobalUnlock(hMem);//�����ڴ��
			CreateStreamOnHGlobal(hMem, TRUE, &stream);//��ָ���ڴ洴�������� TRUE���Զ��ͷ��ڴ�

			_imagedata(stream,_lParam);




			//CImage image;
			//image.Load(stream);

			// Drawing
			//SetStretchBltMode(pDC->GetSafeHdc() , COLORONCOLOR);
			//image.StretchBlt(pDC->GetSafeHdc(),  rect.left,rect.top,rect.Width(),rect.Height(),0,0,image.GetWidth(), image.GetHeight(),SRCCOPY);

			//Save
			//image.Save(path);
			//image.Destroy();

			::GlobalFree(hMem);//�ͷ�ָ����ȫ���ڴ��
		}
	}
};

