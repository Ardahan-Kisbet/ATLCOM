#pragma once

template<class T>
class CProxy_IMyEventObjectEvents :
	public ATL::IConnectionPointImpl<T, &__uuidof(_IMyEventObjectEvents)>
{
public:
	HRESULT Fire_FireEventTest(DOUBLE value)
	{
		HRESULT hr = S_OK;
		T* pThis = static_cast<T*>(this);
		int cConnections = m_vec.GetSize();

		for (int iConnection = 0; iConnection < cConnections; iConnection++)
		{
			pThis->Lock();
			CComPtr<IUnknown> punkConnection = m_vec.GetAt(iConnection);
			pThis->Unlock();

			IDispatch* pConnection = static_cast<IDispatch*>(punkConnection.p);

			if (pConnection)
			{
				CComVariant varParams[1];
				varParams[0] = value;
				varParams[0].vt = VT_R8;
				CComVariant varResult;

				DISPPARAMS params = { varParams, NULL, 1, 0 };
				hr = pConnection->Invoke(1, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &params, &varResult, NULL, NULL);
			}
		}
		return hr;
	}
};
