#pragma once

#include <map>
#include <vector>
#include "DirectShow/streams.h"
#include <initguid.h>
using namespace std;

const long GUID_MAX_COUNT = 1000;
#define WM_GRAPHNOTIFY  (WM_USER+20)

class CSupported
{
private:
	static GUID sSupportedGUIDs[GUID_MAX_COUNT];
	static long sGUIDCount;

public:
	CSupported();
	~CSupported();

	static BOOL IsGUIDSupported(GUID inGUID);
};


class CGraphBuilderCB : public CUnknown
	, public IAMGraphBuilderCallback
{
private:
	CSupported * mSupported;

public:
	CGraphBuilderCB();
	~CGraphBuilderCB();

	DECLARE_IUNKNOWN
	STDMETHODIMP NonDelegatingQueryInterface(REFIID, void**);

	// IAMGraphBuilderCallback
	STDMETHODIMP SelectedFilter(IMoniker *pMon);
	STDMETHODIMP CreatedFilter(IBaseFilter *pFil);
};

class CDXGraph
{
private:
	IGraphBuilder *     mGraph;
	IMediaControl *		mMediaControl;
	IMediaEventEx *		mEvent;
	IBasicVideo	*		mBasicVideo;
	IBasicAudio *		mBasicAudio;
	IVideoWindow  *		mVideoWindow;
	IMediaSeeking *		mSeeking;

	DWORD				mObjectTableEntry;
	bool				m_bBackgroundMusic;
	static long			s_VolumeBack;
	static long			s_Volume;
public:
	CDXGraph();
	virtual ~CDXGraph();

public:
	virtual bool Create(void);						//´´½¨
	virtual void Release(void);						//É¾³ý
	virtual bool Attach(IGraphBuilder * inGraphBuilder); //¿½±´
	void SetBackgroundMusic(bool cyc);

	IGraphBuilder * GetGraph(void); // Not outstanding reference count

	bool ConnectFilters(IPin * inOutputPin, IPin * inInputPin, const AM_MEDIA_TYPE * inMediaType = 0);
	void DisconnectFilters(IPin * inOutputPin);

	bool SetDisplayWindow(HWND inWindow);
	bool SetNotifyWindow(HWND inWindow);
	void HandleEvent(WPARAM inWParam, LPARAM inLParam);

	bool Run(void);        // Control filter graph
	bool Stop(void);
	bool Pause(void);
	bool IsRunning(void);	
	bool IsStopped(void);	
	bool IsPaused(void);	
	void ControlVolume(int ibulk);
	static void	SetVolumeBack(int ibulk);
	static void	SetVolume(int ibulk);

	bool SetFullScreen(BOOL inEnabled);	
	bool GetFullScreen(void);

	bool SetSyncSource(IBaseFilter * inFilter);
	bool SetDefaultSyncSource(void);

	// IMediaSeeking
	double GetCurrentPosition();
	bool SetCurrentPosition(double inPosition);
	double GetDuration();

	bool RenderFile(LPCTSTR inFile);
	bool WaitForCompletion(void);

private:
	void AddToObjectTable(void) ;
	void RemoveFromObjectTable(void);
	bool QueryInterfaces(void);
};