// dllmain.h : Declaration of module class.

class CATLCOMModule : public ATL::CAtlDllModuleT< CATLCOMModule >
{
public :
	DECLARE_LIBID(LIBID_ATLCOMLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_ATLCOM, "{b064163c-52ed-434f-bb57-d2eaf7c2367d}")
};

extern class CATLCOMModule _AtlModule;
