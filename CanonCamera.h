#pragma once
#include <map>
#include "EDSDK/Header/EDSDK.h"
#pragma comment (lib,"EDSDK/Library/EDSDK.lib")

#include "Camrea/CameraModel.h"
#include "Camrea/CameraModelLegacy.h"
#include "Camrea/CameraController.h"
#include "Camrea/CameraEventListener.h"
#include "Class/ActionSource.h"
#include "Event/EVfDownload.h"
#include "Event/DownloadPic.h"
#include "Event/Tv.h"
#include "Event/Av.h"
#include "Event/Iso.h"
#include "Event/AEMode.h"
#include "Event/MeteringMode.h"
#include "Event/ExposureComp.h"
#include "Event/ImageQuality.h"
#include "Event/EvfAFMode.h"

class CanonCamera
{
public:
	CanonCamera(void);
	~CanonCamera(void);
public:
	ActionSource	_TakePicture;
	ActionSource	_StartEVF;
	ActionSource	_EndEVF;
	ActionSource	_FocusNear3;
	ActionSource	_FocusNear2;
	ActionSource	_FocusNear1;
	ActionSource	_FocusFar3;
	ActionSource	_FocusFar2;
	ActionSource	_FocusFar1;
	ActionSource	_FocusUp;
	ActionSource	_FocusDown;
	ActionSource	_FocusLeft;
	ActionSource	_FocusRight;
	ActionSource	_ZoomFit;
	ActionSource    _ZoomZoom;

	//ActionSource	_PressingHalfway;//
	//ActionSource	_PressingCompletely;
	//ActionSource	_PressingOff;
	//ActionSource	_EvfAfON;
	//ActionSource	_EvfAfOFF;

	CDownloadPic    _Download;
	CEVfDownload    _DownloadEvf;
	CTv             _Tv;
	CAv				_Av;
	CIso			_Iso;
	CAEMode			_AEMode;
	CMeteringMode   _MeteringMode;//���ģʽ
	CExposureComp   _ExposureComp;//�عⲹ��
	CImageQuality   _ImageQuality;//���ͼ���ʽ
	CEvfAFMode      _EvfAFMode;
public:
	EdsCameraRef		camera;
	CameraModel*		_model;
	CameraController*	_controller;
	bool				isSDKLoaded;
	bool				isConnected;

	//����ӳ���
	std::map<EdsUInt32, const char *> _TvDesc;	
	std::map<EdsUInt32, const char *> _AvDesc;
	std::map<EdsUInt32, const char *> _IsoDesc;
	std::map<EdsUInt32, const char *> _AEDesc;
	std::map<EdsUInt32, const char *> _MeteringModeDesc;
	std::map<EdsUInt32, const char *> _ExposureCompDesc;
	std::map<EdsUInt32, const char *> _ImageQualityDesc;
	std::map<EdsUInt32, const char *> _EvfAFModeDesc;

	//��ȡ����ת̬
	bool Canon_GetCameraConnectedState(void){return isConnected;};
	//����
	bool Canon_ListCamerasAndConnect(void);
	bool Canon_DisConnect(void);
	//���ö����ߺͱ�������
	void Canon_setupListener(ActionListener* listener);
	void Canon_setupObserver(Observable* ob);
	//EVF
	bool Canon_StartEVF(void);
	bool Canon_EndEVF(void);
	//����
	bool Canon_ZoomFit(void);
	bool Canon_ZoomZoom(void);	
	//����
	bool Canon_TakeAnImage(void);	
	//�Խ�
	bool Canon_Focus_Near(int value);
	bool Canon_Focus_Far(int value);
	bool Canon_Focus_Direction(int value);
	//����
	bool Canon_SetTv(int sel);
	//��Ȧ
	bool Canon_SetAv(int sel);
	//�й��
	bool Canon_SetIso(int sel);
	//AE
	bool Canon_SetAEMode(int sel);
	//���ģʽ
	bool Canon_SetAMeteringMode(int sel);
	//�عⲹ��
	bool Canon_SetExposureComp(int sel);
	//���ͼ���ʽ
	bool Canon_SetImageQuality(int sel);
	//���ͼ���ʽ
	bool Canon_SetEvfAFMode(int sel);
	//���ûص�����
	bool Canon_SetDownloadEvfCallback(IMAGEDATACALLBACK imgdata,LPARAM lParam);             //Ԥ��������
	void Canon_SetDownLoadCallback(DOWNLOADPROGRESSCALLBACK downloadstate,LPARAM lParam);   //����ͼ��Ľ��� 
	bool Canon_SetTvCallback(TVCALLBACK tvstate,LPARAM lParam);								//TVֵ						
	bool Canon_SetAvCallback(AVCALLBACK avstate,LPARAM lParam);								//AVֵ
	bool Canon_SetIsoCallback(ISOCALLBACK isostate,LPARAM lParam);							//ISOֵ
	bool Canon_SetAEModeCallback(AECALLBACK aestate,LPARAM lParam);							//AEģʽ
	bool Canon_SetMeteringModeCallback(METERINGMODECALLBACK meteringmodestate,LPARAM lParam);//���ģʽ
	bool Canon_SetExposureCompCallback(EXPOSURECOMPCALLBACK exposurecompstate,LPARAM lParam);//�عⲹ��
	bool Canon_SetImageQualityCallback(IMGQUALITYCALLBACK imgqualitystate,LPARAM lParam);	//ͼ������
	bool Canon_SetEvfAFModeCallback(EVFAFMODECALLBACK evfafmodestate,LPARAM lParam);        //EVF AFģʽ


	CameraController* getController(){return _controller;};
	CameraModel* getModel(){return _model;};
	BOOL IsValidate(void) {return (_controller == NULL || _model == NULL)?false:true;};
	CameraModel* cameraModelFactory(EdsCameraRef camera, EdsDeviceInfo deviceInfo)
	{
		// if Legacy protocol.
		if(deviceInfo.deviceSubType == 0)
		{
			return new CameraModelLegacy(camera);//��Э��
		}

		// PTP protocol.
		return new CameraModel(camera);
	}
};

