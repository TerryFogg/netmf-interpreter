//-----------------------------------------------------------------------------
//
//    ** DO NOT EDIT THIS FILE! **
//    This file was generated by a tool
//    re-running the tool will overwrite this file.
//
//-----------------------------------------------------------------------------

#define _XKEYCHECK_H

#include <alljoyn.h>
#include <aj_guid.h>    // for AJ_CreateNewGUID
//#include <aj_creds.h>   // for AJ_GetLocalGUID
#include <aj_crypto.h>  // for AJ_RandBytes
#include <aj_helper.h>

#include <TinyCLR_Runtime.h>
#include <TinyCLR_Checks.h>
#include <TinyCLR_Diagnostics.h>
#include <TinyCLR_Graphics.h>
#include <TinyCLR_Hardware.h>
#include <TinyCLR_Interop.h>
#include <TinyCLR_Version.h>



#include "spot_alljoyn_native.h"
#include "spot_alljoyn_native_Microsoft_SPOT_AllJoyn_AJ.h"

#define AJ_InfoPrintf(_msg)

using namespace Microsoft::SPOT::AllJoyn;

extern AJ_Status MarshalDefaultProps(AJ_Message* msg);
extern AJ_Status MarshalObjectDescriptions(AJ_Message* msg);

static AJ_BusAttachment BusInstance;
static AJ_Arg ArgPool[10];

HRESULT Library_spot_alljoyn_native_Microsoft_SPOT_AllJoyn_AJ::Initialize___U4( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER(); hr = S_OK;
    {
        AJ_Initialize();
		UINT32 retVal = 1;
        TINYCLR_CHECK_HRESULT( hr );
        SetResult_UINT32( stack, 1 );
    }
    TINYCLR_NOCLEANUP();
}

AJ_Status CustomStartService(AJ_BusAttachment* bus,
                          const char* daemonName,
                          uint32_t timeout,
                          uint8_t connected,
                          uint16_t port,
                          const char* name,
                          uint32_t flags,
                          const AJ_SessionOpts* opts
                          )
{
    AJ_Status status;
    AJ_Time timer;
    uint8_t serviceStarted = FALSE;

    AJ_InfoPrintf(("AJ_StartService(bus=0x%p, daemonName=\"%s\", timeout=%d., connected=%d., port=%d., name=\"%s\", flags=0x%x, opts=0x%p)\n",
                   bus, daemonName, timeout, connected, port, name, flags, opts));

    AJ_InitTimer(&timer);

    while (TRUE) {
        if (AJ_GetElapsedTime(&timer, TRUE) > timeout) {
            return AJ_ERR_TIMEOUT;
        }
        if (!connected) {
            AJ_InfoPrintf(("AJ_StartService(): AJ_FindBusAndConnect()\n"));
            status = AJ_FindBusAndConnect(bus, daemonName, AJ_CONNECT_TIMEOUT);
            if (status != AJ_OK) {
                AJ_WarnPrintf(("AJ_StartService(): connect failed: sleeping for %d seconds\n", AJ_CONNECT_PAUSE / 1000));
                AJ_Sleep(AJ_CONNECT_PAUSE);
                continue;
            }
            AJ_InfoPrintf(("AJ_StartService(): connected to bus\n"));
        }
        /*
         * Kick things off by binding a session port
         */
        AJ_InfoPrintf(("AJ_StartService(): AJ_BindSessionPort()\n"));
        status = AJ_BusBindSessionPort(bus, port, opts, 0);
        if (status == AJ_OK) {
            break;
        }
        AJ_ErrPrintf(("AJ_StartService(): AJ_Disconnect(): status=%s.\n", AJ_StatusText(status)));
        AJ_Disconnect(bus);
    }

    while (!serviceStarted && (status == AJ_OK)) {
        AJ_Message msg;

        status = AJ_UnmarshalMsg(bus, &msg, AJ_UNMARSHAL_TIMEOUT);
        if (status == AJ_ERR_NO_MATCH) {
            // Ignore unknown messages
            status = AJ_OK;
            continue;
        }
        if (status != AJ_OK) {
            AJ_ErrPrintf(("AJ_StartService(): status=%s.\n", AJ_StatusText(status)));
            break;
        }

        switch (msg.msgId) {
        case AJ_REPLY_ID(AJ_METHOD_BIND_SESSION_PORT):
            if (msg.hdr->msgType == AJ_MSG_ERROR) {
                AJ_ErrPrintf(("AJ_StartService(): AJ_METHOD_BIND_SESSION_PORT: %s\n", msg.error));
                status = AJ_ERR_FAILURE;
            } else {
                AJ_InfoPrintf(("AJ_StartService(): AJ_BusRequestName()\n"));
                status = AJ_BusRequestName(bus, name, flags);
            }
            break;

        case AJ_REPLY_ID(AJ_METHOD_REQUEST_NAME):
            if (msg.hdr->msgType == AJ_MSG_ERROR) {
                AJ_ErrPrintf(("AJ_StartService(): AJ_METHOD_REQUEST_NAME: %s\n", msg.error));
                status = AJ_ERR_FAILURE;
            } else {
                AJ_InfoPrintf(("AJ_StartService(): AJ_BusAdvertiseName()\n"));
                status = AJ_BusAdvertiseName(bus, name, (opts != NULL) ? opts->transports : AJ_TRANSPORT_ANY, AJ_BUS_START_ADVERTISING, 0);
            }
            break;

        case AJ_REPLY_ID(AJ_METHOD_ADVERTISE_NAME):
            if (msg.hdr->msgType == AJ_MSG_ERROR) {
                AJ_ErrPrintf(("AJ_StartService(): AJ_METHOD_ADVERTISE_NAME: %s\n", msg.error));
                status = AJ_ERR_FAILURE;
            } else {
                serviceStarted = TRUE;
            }
            break;

        default:
            /*
             * Pass to the built-in bus message handlers
             */
            AJ_InfoPrintf(("AJ_StartService(): AJ_BusHandleBusMessage()\n"));
            status = AJ_BusHandleBusMessage(&msg);
            break;
        }
        AJ_CloseMsg(&msg);
    }

    if (status == AJ_OK) {
        //status = AJ_AboutInit(bus, port);
    } else {
        AJ_WarnPrintf(("AJ_StartService(): AJ_Disconnect(): status=%s\n", AJ_StatusText(status)));
        AJ_Disconnect(bus);
    }
    return status;
}

HRESULT Library_spot_alljoyn_native_Microsoft_SPOT_AllJoyn_AJ::StartService___MicrosoftSPOTAllJoynAJStatus__U4__STRING__U4__I1__U2__STRING__U4__I4( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER();
    {
		UINT32 param0 = stack.Arg1().NumericByRef().u4;
		CLR_RT_HeapBlock_String * param1 = stack.Arg2().DereferenceString();
		UINT32 param2 = stack.Arg3().NumericByRef().u4;
		INT8 param3   = stack.Arg4().NumericByRef().s1;
		UINT16 param4 = stack.Arg5().NumericByRef().s2;
		CLR_RT_HeapBlock_String * param5 = stack.Arg6().DereferenceString();
		UINT32 param6 = stack.Arg7().NumericByRef().u4;
		INT32 param7 = stack.ArgN(8).NumericByRef().s4;

		INT32 retVal = CustomStartService((AJ_BusAttachment*)param0, NULL, param2, param3, param4, param5->StringText(), param6, NULL);
        SetResult_INT32( stack, retVal );

    }
    TINYCLR_NOCLEANUP();
}

HRESULT Library_spot_alljoyn_native_Microsoft_SPOT_AllJoyn_AJ::AlwaysPrintf___VOID__STRING( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER();
    {
		CLR_RT_HeapBlock_String * param0 = stack.Arg1().DereferenceString();
		AJ_AlwaysPrintf((param0->StringText()));
    }
    TINYCLR_NOCLEANUP();
}

void CopyFromManagedMsg(CLR_RT_HeapBlock* managedMsg, AJ_Message* msg)
{
    typedef Library_spot_alljoyn_native_Microsoft_SPOT_AllJoyn_AJ_Message Managed_AJ_Message;

    msg->msgId          =                     managedMsg[ Managed_AJ_Message::FIELD__msgId         ].NumericByRef().u4;
    msg->hdr            = (AJ_MsgHeader*)     managedMsg[ Managed_AJ_Message::FIELD__hdr           ].NumericByRef().u4;
    msg->replySerial    =                     managedMsg[ Managed_AJ_Message::FIELD__union0        ].NumericByRef().u4;
    msg->member         = (char*)             managedMsg[ Managed_AJ_Message::FIELD__union1        ].NumericByRef().u4;
    msg->iface          = (char*)             managedMsg[ Managed_AJ_Message::FIELD__iface         ].NumericByRef().u4;
    msg->sender         = (char*)             managedMsg[ Managed_AJ_Message::FIELD__sender        ].NumericByRef().u4;
    msg->destination    = (char*)             managedMsg[ Managed_AJ_Message::FIELD__destination   ].NumericByRef().u4;
    msg->signature      = (char*)             managedMsg[ Managed_AJ_Message::FIELD__signature     ].NumericByRef().u4;
    msg->sessionId      =                     managedMsg[ Managed_AJ_Message::FIELD__sessionId     ].NumericByRef().u4;
    msg->timestamp      =                     managedMsg[ Managed_AJ_Message::FIELD__timestamp     ].NumericByRef().u4;
    msg->ttl            =                     managedMsg[ Managed_AJ_Message::FIELD__ttl           ].NumericByRef().u4;
    msg->sigOffset      =                     managedMsg[ Managed_AJ_Message::FIELD__sigOffset     ].NumericByRef().u1;
    msg->varOffset      =                     managedMsg[ Managed_AJ_Message::FIELD__varOffset     ].NumericByRef().u1;
    msg->bodyBytes      =                     managedMsg[ Managed_AJ_Message::FIELD__bodyBytes     ].NumericByRef().u2;
    msg->bus            = (AJ_BusAttachment*) managedMsg[ Managed_AJ_Message::FIELD__bus           ].NumericByRef().u4;
    msg->outer          = (struct _AJ_Arg*)   managedMsg[ Managed_AJ_Message::FIELD__outer         ].NumericByRef().u4;        
}

void CopyToManagedMsg(CLR_RT_HeapBlock* managedMsg, AJ_Message* msg)
{
    typedef Library_spot_alljoyn_native_Microsoft_SPOT_AllJoyn_AJ_Message Managed_AJ_Message;
    
    managedMsg[ Managed_AJ_Message::FIELD__msgId         ].SetInteger(msg->msgId);
    managedMsg[ Managed_AJ_Message::FIELD__hdr           ].SetInteger((unsigned int)msg->hdr);
    managedMsg[ Managed_AJ_Message::FIELD__union0        ].SetInteger(msg->replySerial);
    managedMsg[ Managed_AJ_Message::FIELD__union1        ].SetInteger((unsigned int)msg->member);
    managedMsg[ Managed_AJ_Message::FIELD__iface         ].SetInteger((unsigned int)msg->iface);
    managedMsg[ Managed_AJ_Message::FIELD__sender        ].SetInteger((unsigned int)msg->sender);
    managedMsg[ Managed_AJ_Message::FIELD__destination   ].SetInteger((unsigned int)msg->destination);
    managedMsg[ Managed_AJ_Message::FIELD__signature     ].SetInteger((unsigned int)msg->signature);
    managedMsg[ Managed_AJ_Message::FIELD__sessionId     ].SetInteger(msg->sessionId);
    managedMsg[ Managed_AJ_Message::FIELD__timestamp     ].SetInteger(msg->timestamp);
    managedMsg[ Managed_AJ_Message::FIELD__ttl           ].SetInteger(msg->ttl);
    managedMsg[ Managed_AJ_Message::FIELD__sigOffset     ].SetInteger(msg->sigOffset);
    managedMsg[ Managed_AJ_Message::FIELD__varOffset     ].SetInteger(msg->varOffset);
    managedMsg[ Managed_AJ_Message::FIELD__bodyBytes     ].SetInteger(msg->bodyBytes);
    managedMsg[ Managed_AJ_Message::FIELD__bus           ].SetInteger((unsigned int)msg->bus);
    managedMsg[ Managed_AJ_Message::FIELD__outer         ].SetInteger((unsigned int)msg->outer);
}

HRESULT Library_spot_alljoyn_native_Microsoft_SPOT_AllJoyn_AJ::MarshalReplyMsg___MicrosoftSPOTAllJoynAJStatus__MicrosoftSPOTAllJoynAJMessage__MicrosoftSPOTAllJoynAJMessage( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER(); hr = S_OK;
    {
        AJ_Message replyMsg;
        AJ_Message msg;

        CLR_RT_HeapBlock* managedMsg = stack.Arg1().Dereference();  FAULT_ON_NULL(managedMsg); 
        CopyFromManagedMsg(managedMsg, &msg);
        
        AJ_Status status = AJ_MarshalReplyMsg(&msg, &replyMsg);
        
        CLR_RT_HeapBlock* managedReplyMsg = stack.Arg2().Dereference();  FAULT_ON_NULL(managedReplyMsg);    
        
        CopyToManagedMsg(managedReplyMsg, &replyMsg);
        CopyToManagedMsg(managedMsg, &msg);
        
        TINYCLR_CHECK_HRESULT( hr );
        SetResult_INT32( stack, status );
    }
    TINYCLR_NOCLEANUP();
}

HRESULT Library_spot_alljoyn_native_Microsoft_SPOT_AllJoyn_AJ::DeliverMsg___MicrosoftSPOTAllJoynAJStatus__MicrosoftSPOTAllJoynAJMessage( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER(); hr = S_OK;
    {
        AJ_Message replyMsg;
        CLR_RT_HeapBlock* managedReplyMsg = stack.Arg1().Dereference();  FAULT_ON_NULL(managedReplyMsg);            
                
        CopyFromManagedMsg(managedReplyMsg, &replyMsg);
        
        AJ_Status status = AJ_DeliverMsg(&replyMsg);
        TINYCLR_CHECK_HRESULT( hr );
        SetResult_INT32( stack, status );

    }
    TINYCLR_NOCLEANUP();
}

HRESULT Library_spot_alljoyn_native_Microsoft_SPOT_AllJoyn_AJ::MarshalSignal___MicrosoftSPOTAllJoynAJStatus__U4__MicrosoftSPOTAllJoynAJMessage__U4__U4__U4__U1__U4( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER(); hr = S_OK;
    {
        AJ_BusAttachment * bus              = (AJ_BusAttachment *) stack.Arg1().NumericByRef().u4;  FAULT_ON_NULL(bus);               
        CLR_RT_HeapBlock * managedMsg       = stack.Arg2().Dereference();  FAULT_ON_NULL(managedMsg);    
        uint32_t msgId                      = stack.Arg3().NumericByRef().u4;
        const char * destination            = (const char *) stack.Arg4().NumericByRef().u4;
        AJ_SessionId sessionId              = (AJ_SessionId) stack.Arg5().NumericByRef().u4;
        uint8_t flags                       = stack.Arg6().NumericByRef().u1;
        uint32_t ttl                        = stack.Arg7().NumericByRef().u4;    

        AJ_Message msg;        
        AJ_Status status = AJ_MarshalSignal(bus, &msg, msgId, destination, sessionId, flags, ttl);
        
        CopyToManagedMsg(managedMsg, &msg);
        
        TINYCLR_CHECK_HRESULT( hr );
        SetResult_INT32( stack, status );

    }
    TINYCLR_NOCLEANUP();
}


HRESULT Library_spot_alljoyn_native_Microsoft_SPOT_AllJoyn_AJ::MarshalArg___MicrosoftSPOTAllJoynAJStatus__MicrosoftSPOTAllJoynAJMessage__STRING__U4( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER(); hr = S_OK;
    {
        AJ_Message msg;
        CLR_RT_HeapBlock * managedMsg = stack.Arg1().Dereference();  FAULT_ON_NULL(managedMsg);
                
        CopyFromManagedMsg(managedMsg, &msg);
        
        CLR_RT_HeapBlock_String * signature = stack.Arg2().DereferenceString();  FAULT_ON_NULL(signature);
        uint32_t arg = stack.Arg3().NumericByRef().u4;

        AJ_Status status = AJ_MarshalArgs(&msg, signature->StringText(), arg);
        
        CopyToManagedMsg(managedMsg, &msg);
        
        TINYCLR_CHECK_HRESULT( hr );
        SetResult_INT32( stack, status );

    }
    TINYCLR_NOCLEANUP();
}

HRESULT Library_spot_alljoyn_native_Microsoft_SPOT_AllJoyn_AJ::MarshalArg___MicrosoftSPOTAllJoynAJStatus__MicrosoftSPOTAllJoynAJMessage__STRING__STRING( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER(); hr = S_OK;
    {
        AJ_Message msg;
        CLR_RT_HeapBlock * managedMsg = stack.Arg1().Dereference();  FAULT_ON_NULL(managedMsg);     

        CopyFromManagedMsg(managedMsg, &msg);
        
        CLR_RT_HeapBlock_String * sig = stack.Arg2().DereferenceString();        
        CLR_RT_HeapBlock_String * arg = stack.Arg3().DereferenceString();

        AJ_Status status = AJ_MarshalArgs(&msg, sig->StringText(), arg->StringText());

        CopyToManagedMsg(managedMsg, &msg);
                
        TINYCLR_CHECK_HRESULT( hr );
        SetResult_INT32( stack, status );

    }
    TINYCLR_NOCLEANUP();
}

HRESULT Library_spot_alljoyn_native_Microsoft_SPOT_AllJoyn_AJ::MarshalArgs___MicrosoftSPOTAllJoynAJStatus__MicrosoftSPOTAllJoynAJMessage__STRING__STRING__STRING__STRING( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER(); hr = S_OK;
    {
        AJ_Message msg;
        CLR_RT_HeapBlock * managedMsg = stack.Arg1().Dereference();  FAULT_ON_NULL(managedMsg);     

        CopyFromManagedMsg(managedMsg, &msg);
        
        CLR_RT_HeapBlock_String * sig  = stack.Arg2().DereferenceString();
        CLR_RT_HeapBlock_String * arg1 = stack.Arg3().DereferenceString();
        CLR_RT_HeapBlock_String * arg2 = stack.Arg4().DereferenceString();
        CLR_RT_HeapBlock_String * arg3 = stack.Arg5().DereferenceString();

        const char * a = sig->StringText();
        const char * b = arg1->StringText();
        const char * c = arg2->StringText();
        const char * d = arg3->StringText();
                
        AJ_Status status = AJ_MarshalArgs(&msg, a, b, c, d);
        
        CopyToManagedMsg(managedMsg, &msg);
        
        TINYCLR_CHECK_HRESULT( hr );
        SetResult_INT32( stack, status );

    }
    TINYCLR_NOCLEANUP();
}

HRESULT Library_spot_alljoyn_native_Microsoft_SPOT_AllJoyn_AJ::MarshalArgs___MicrosoftSPOTAllJoynAJStatus__MicrosoftSPOTAllJoynAJMessage__STRING__STRING__SZARRAY_U1( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER(); hr = S_OK;
    {
        AJ_Message msg;
        CLR_RT_HeapBlock * managedMsg = stack.Arg1().Dereference();  FAULT_ON_NULL(managedMsg);     

        CopyFromManagedMsg(managedMsg, &msg);
                
        CLR_RT_HeapBlock_String * sig = stack.Arg2().DereferenceString();
        CLR_RT_HeapBlock_String * arg1 = stack.Arg3().DereferenceString();
        CLR_RT_HeapBlock_Array * arg2 = stack.Arg4().DereferenceArray();
        
        AJ_Status status = AJ_MarshalArgs(&msg, sig->StringText(), arg1->StringText(), (char*)arg2->GetFirstElement(), arg2->m_numOfElements);
        
        CopyToManagedMsg(managedMsg, &msg);
        
        TINYCLR_CHECK_HRESULT( hr );
        SetResult_INT32( stack, status );

    }
    TINYCLR_NOCLEANUP();
}

AJ_Status AboutIconHandleGetContent(AJ_Message* msg, AJ_Message* reply, char* data, int dataSize)
{
    AJ_Status status;
    uint32_t u = (uint32_t)dataSize;

    status = AJ_MarshalReplyMsg(msg, reply);
    if (status != AJ_OK) {
        goto ErrorExit;
    }
    status = AJ_DeliverMsgPartial(reply, u + 4);
    if (status != AJ_OK) {
        goto ErrorExit;
    }
    status = AJ_MarshalRaw(reply, &u, 4);
    if (status != AJ_OK) {
        goto ErrorExit;
    }
    return AJ_MarshalRaw(reply, data, u);

ErrorExit:
    return status;
}

HRESULT Library_spot_alljoyn_native_Microsoft_SPOT_AllJoyn_AJ::AboutIconHandleGetContent___MicrosoftSPOTAllJoynAJStatus__MicrosoftSPOTAllJoynAJMessage__MicrosoftSPOTAllJoynAJMessage__SZARRAY_U1( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER(); hr = S_OK;
    {
        AJ_Message msg;
        AJ_Message replyMsg;
        
        CLR_RT_HeapBlock * managedMsg = stack.Arg1().Dereference();  FAULT_ON_NULL(managedMsg);
        CLR_RT_HeapBlock * managedReplyMsg = stack.Arg2().Dereference();  FAULT_ON_NULL(managedReplyMsg);

        CopyFromManagedMsg(managedMsg, &msg);
        CopyFromManagedMsg(managedReplyMsg, &replyMsg);
        
        CLR_RT_HeapBlock_Array * data = stack.Arg3().DereferenceArray();        

        INT32 retVal = AboutIconHandleGetContent( &msg,  &replyMsg, (char*)data->GetFirstElement(), data->m_numOfElements);
        
        CopyToManagedMsg(managedMsg, &msg);
        CopyToManagedMsg(managedReplyMsg, &replyMsg);
        
        TINYCLR_CHECK_HRESULT( hr );
        SetResult_INT32( stack, retVal );

    }
    TINYCLR_NOCLEANUP();
}
HRESULT Library_spot_alljoyn_native_Microsoft_SPOT_AllJoyn_PropertyStore::GetLocalGUID___MicrosoftSPOTAllJoynAJStatus__SZARRAY_U1( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER(); hr = S_OK;
    {
        AJ_GUID theAJ_GUID;
        AJ_Status status = AJ_OK;
        AJ_CreateNewGUID((uint8_t*)&theAJ_GUID, sizeof(AJ_GUID));
    
        CLR_RT_HeapBlock_Array * data = stack.Arg1().DereferenceArray();
    
        char * a = (char*) data->GetFirstElement();
        char * p = (char*) &theAJ_GUID;

        for(int i=0; i<data->m_numOfElements; i++)
        {
            a[i] = p[i];
        }
        
        TINYCLR_CHECK_HRESULT( hr );
        SetResult_INT32( stack, status );

    }
    TINYCLR_NOCLEANUP();
}

HRESULT Library_spot_alljoyn_native_Microsoft_SPOT_AllJoyn_AJ::GetArgPtr___U4__I4( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER(); hr = S_OK;
    {

        INT32 idx = stack.Arg1().NumericByRef().s4;

        UINT32 ret = (UINT32)&ArgPool[idx];
        
        TINYCLR_CHECK_HRESULT( hr );
        SetResult_UINT32( stack, ret );

    }
    TINYCLR_NOCLEANUP();
}

HRESULT Library_spot_alljoyn_native_Microsoft_SPOT_AllJoyn_AJ::MarshalContainer___MicrosoftSPOTAllJoynAJStatus__MicrosoftSPOTAllJoynAJMessage__U4__U1( CLR_RT_StackFrame& stack )
{

    TINYCLR_HEADER(); hr = S_OK;
    {
        AJ_Message msg;
        CLR_RT_HeapBlock * managedMsg = stack.Arg1().Dereference();  FAULT_ON_NULL(managedMsg);     
    
        CopyFromManagedMsg(managedMsg, &msg);
        
        AJ_Arg * arg = (AJ_Arg*) stack.Arg2().NumericByRef().s4;        
        uint8_t typeId = stack.Arg3().NumericByRef().u1;

        AJ_Status status = AJ_MarshalContainer(&msg, arg, typeId);
                
        CopyToManagedMsg(managedMsg, &msg);
        
        TINYCLR_CHECK_HRESULT( hr );
        SetResult_INT32( stack, status );

    }
    TINYCLR_NOCLEANUP();
}

HRESULT Library_spot_alljoyn_native_Microsoft_SPOT_AllJoyn_AJ::MarshalCloseContainer___MicrosoftSPOTAllJoynAJStatus__MicrosoftSPOTAllJoynAJMessage__U4( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER(); hr = S_OK;
    {
        AJ_Message msg;
        CLR_RT_HeapBlock * managedMsg = stack.Arg1().Dereference();  FAULT_ON_NULL(managedMsg);     
    
        CopyFromManagedMsg(managedMsg, &msg);
        
        AJ_Arg * arg = (AJ_Arg*) stack.Arg2().NumericByRef().u4;                     
        AJ_Status status = AJ_MarshalCloseContainer(&msg, arg);
        
        CopyToManagedMsg(managedMsg, &msg);
        
        TINYCLR_CHECK_HRESULT( hr );
        SetResult_INT32( stack, status );
    }
    TINYCLR_NOCLEANUP();
}

HRESULT Library_spot_alljoyn_native_Microsoft_SPOT_AllJoyn_AJ::MarshalObjectDescriptions___MicrosoftSPOTAllJoynAJStatus__MicrosoftSPOTAllJoynAJMessage( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER(); hr = S_OK;
    {
        AJ_Message msg;
        CLR_RT_HeapBlock * managedMsg = stack.Arg1().Dereference();  FAULT_ON_NULL(managedMsg);     

        CopyFromManagedMsg(managedMsg, &msg);
        
        INT32 retVal = MarshalObjectDescriptions(&msg);
        
        CopyToManagedMsg(managedMsg, &msg);        
        
        TINYCLR_CHECK_HRESULT( hr );
        SetResult_INT32( stack, retVal );

    }
    TINYCLR_NOCLEANUP();
}

HRESULT Library_spot_alljoyn_native_Microsoft_SPOT_AllJoyn_AJ::MarshalDefaultProps___MicrosoftSPOTAllJoynAJStatus__MicrosoftSPOTAllJoynAJMessage( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER(); hr = S_OK;
    {
        AJ_Message msg;
        CLR_RT_HeapBlock * managedMsg = stack.Arg1().Dereference();  FAULT_ON_NULL(managedMsg);                
        
        CopyFromManagedMsg(managedMsg, &msg);
        
        INT32 retVal = MarshalDefaultProps(&msg);
        
        CopyToManagedMsg(managedMsg, &msg);
        
        TINYCLR_CHECK_HRESULT( hr );
        SetResult_INT32( stack, retVal );

    }
    TINYCLR_NOCLEANUP();
}

#define MAX_DIM_INTERFACE 100
#define MAX_NUM_INTERFACE 100


void DeserializeInterfaceString(const char * data, char * testinterface[])
{
    const char * p = data;
    const char * cur = p;
    
	char * interfaceString[MAX_DIM_INTERFACE];
	char delimiter = ',';
	int pos = 0;
	int i = 0;

	while (*p != NULL)
	{
        if (*p == delimiter)
        {
            // found a full string, copy it to array
            
            int size = p - cur + 1;  // extra byte for null
            testinterface[i] = new char[size];

            for (int j=0; j<size; j++){
                testinterface[i][j] = cur[j];
            }
            testinterface[i][size-1] = '\0';
            
            i ++;            
            cur = p+1;
        }
        p ++;
	}
	testinterface[i - 1] = '\0';
}

HRESULT Library_spot_alljoyn_native_Microsoft_SPOT_AllJoyn_AJ::RegisterObjects___STATIC__VOID__STRING__STRING__U1__I4__BOOLEAN( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER(); hr = S_OK;
    {
        const int MAX_OBJS = 5;
        static char* localInterface[MAX_DIM_INTERFACE] = {0,};
    
        CLR_RT_HeapBlock_String * pLocalPath = stack.Arg0().DereferenceString();        
        CLR_RT_HeapBlock_String * pLocalInterfaces = stack.Arg1().DereferenceString();
                
        uint8_t localFlags = stack.Arg2().NumericByRef().u1;
        uint8_t localContext = stack.Arg3().NumericByRef().u4;
        
        bool useProperties = !! stack.Arg4().NumericByRef().u1;
        
        const char * path = pLocalPath->StringText();
        const char * ifaces = pLocalInterfaces->StringText();

        DeserializeInterfaceString(ifaces, localInterface);
        
        if (useProperties == true)
        {
            static const AJ_InterfaceDescription localInterfaces[] = {
                AJ_PropertiesIface,     // This must be included for any interface that has properties. 
                localInterface,
                NULL
            };    
            
            static const AJ_Object appLocalObjects[] = {
                { path, localInterfaces,  AJ_OBJ_FLAG_ANNOUNCED | AJ_OBJ_FLAG_DESCRIBED  },  // make them announceable
                { NULL }
            };
            
            AJ_RegisterObjects(appLocalObjects, NULL);
            AJ_PrintXML(appLocalObjects);
        }
        else
        {
            static const AJ_InterfaceDescription localInterfaces[] = {
                localInterface,
                NULL
            };
            
            static const AJ_Object appLocalObjects[] = {
                { path, localInterfaces, AJ_OBJ_FLAG_ANNOUNCED | AJ_OBJ_FLAG_DESCRIBED  },  // make them announceable
                { NULL }
            };
            
            AJ_RegisterObjects(appLocalObjects, NULL);
        }
        
        TINYCLR_CHECK_HRESULT( hr );
    }
    TINYCLR_NOCLEANUP();
}


HRESULT Library_spot_alljoyn_native_Microsoft_SPOT_AllJoyn_AJ::UnmarshalVariant___STRING__MicrosoftSPOTAllJoynAJMessage( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER(); hr = S_OK;
    {
        AJ_Message msg;
        CLR_RT_HeapBlock * managedMsg = stack.Arg1().Dereference();  FAULT_ON_NULL(managedMsg);                
        
        CopyFromManagedMsg(managedMsg, &msg);
        
        const char* variant;
        AJ_UnmarshalVariant(&msg, &variant);
        
        CopyToManagedMsg(managedMsg, &msg);
        
        TINYCLR_CHECK_HRESULT( hr );
        SetResult_LPCSTR( stack, variant );
    }
    TINYCLR_NOCLEANUP();
}

HRESULT Library_spot_alljoyn_native_Microsoft_SPOT_AllJoyn_AJ::MarshalVariant___MicrosoftSPOTAllJoynAJStatus__MicrosoftSPOTAllJoynAJMessage__STRING( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER(); hr = S_OK;
    {
        AJ_Message msg;
        CLR_RT_HeapBlock * managedMsg = stack.Arg1().Dereference();  FAULT_ON_NULL(managedMsg);                
        CopyFromManagedMsg(managedMsg, &msg);        
        
        CLR_RT_HeapBlock_String * signature = stack.Arg2().DereferenceString();  FAULT_ON_NULL(signature);

        AJ_Status status = AJ_MarshalVariant(&msg, signature->StringText());
        
        CopyToManagedMsg(managedMsg, &msg);
        
        TINYCLR_CHECK_HRESULT( hr );
        SetResult_INT32( stack, status );

    }
    TINYCLR_NOCLEANUP();
}

HRESULT Library_spot_alljoyn_native_Microsoft_SPOT_AllJoyn_AJ::UnmarshalPropertyArgs___STRING__MicrosoftSPOTAllJoynAJMessage__BYREF_U4( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER(); hr = S_OK;
    {
        AJ_Message msg;
        CLR_RT_HeapBlock * managedMsg = stack.Arg1().Dereference();  FAULT_ON_NULL(managedMsg);                
        
        CopyFromManagedMsg(managedMsg, &msg);

        UINT32 * param1;
        UINT8 heapblock1[CLR_RT_HEAP_BLOCK_SIZE];
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UINT32_ByRef( stack, heapblock1, 2, param1 ) );

        const char * sig = 0;
        uint32_t propId = 0;
        AJ_Status status = AJ_UnmarshalPropertyArgs(&msg, &propId, &sig);
        
        *param1 = propId;
        
        CopyToManagedMsg(managedMsg, &msg);
        
        TINYCLR_CHECK_HRESULT( hr );
        SetResult_LPCSTR( stack, sig );

        TINYCLR_CHECK_HRESULT( Interop_Marshal_StoreRef( stack, heapblock1, 2 ) );
    }
    TINYCLR_NOCLEANUP();
}


HRESULT Library_spot_alljoyn_native_Microsoft_SPOT_AllJoyn_AJ::BusSetPasswordCallback___VOID( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER(); hr = S_OK;
    {
        TINYCLR_CHECK_HRESULT( hr );
    }
    TINYCLR_NOCLEANUP();
}

HRESULT Library_spot_alljoyn_native_Microsoft_SPOT_AllJoyn_AJ::UnmarshalMsg___MicrosoftSPOTAllJoynAJStatus__U4__MicrosoftSPOTAllJoynAJMessage__U4( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER(); hr = S_OK;
    {
		UINT32 param0 = stack.Arg1().NumericByRef().u4;
		
        AJ_Message msg;
        CLR_RT_HeapBlock * managedMsg = stack.Arg2().Dereference();  FAULT_ON_NULL(managedMsg);                
        
        CopyFromManagedMsg(managedMsg, &msg);
        
		UINT32 param3 = stack.Arg3().NumericByRef().u4;

        UINT32 * param2;
        UINT8 heapblock2[CLR_RT_HEAP_BLOCK_SIZE];
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UINT32_ByRef( stack, heapblock2, 3, param2 ) );

		AJ_Status status = AJ_OK;

		status = AJ_UnmarshalMsg((AJ_BusAttachment *)param0, &msg, param3);

		*param2 = msg.msgId;

        CopyToManagedMsg(managedMsg, &msg);
        
        TINYCLR_CHECK_HRESULT( Interop_Marshal_StoreRef( stack, heapblock2, 3 ) );
        TINYCLR_CHECK_HRESULT( hr );
        SetResult_INT32( stack, (INT32)status );
    }
    TINYCLR_NOCLEANUP();
}

HRESULT Library_spot_alljoyn_native_Microsoft_SPOT_AllJoyn_AJ::UnmarshalArgs___MicrosoftSPOTAllJoynAJStatus__MicrosoftSPOTAllJoynAJMessage__STRING__U2__U4__STRING( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER(); hr = S_OK;
    {
        AJ_Message msg;
        CLR_RT_HeapBlock * managedMsg = stack.Arg1().Dereference();  FAULT_ON_NULL(managedMsg);                
        
        CopyFromManagedMsg(managedMsg, &msg);
        
        CLR_RT_HeapBlock_String *  param1 = stack.Arg2().DereferenceString();
        UINT16 param2 = stack.Arg3().NumericByRef().u2;
        UINT32 param3 = stack.Arg4().NumericByRef().u4;
        UINT32 param4 = stack.Arg5().NumericByRef().u4;

        AJ_Status status = AJ_UnmarshalArgs(&msg, param1->StringText(), &param2, &param3, &param4);
        
        CopyToManagedMsg(managedMsg, &msg);
        TINYCLR_CHECK_HRESULT( hr );
        SetResult_INT32( stack, (INT32)status );

    }
    TINYCLR_NOCLEANUP();
}

HRESULT Library_spot_alljoyn_native_Microsoft_SPOT_AllJoyn_AJ::UnmarshalArgs___MicrosoftSPOTAllJoynAJStatus__MicrosoftSPOTAllJoynAJMessage__STRING__BYREF_U4( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER(); hr = S_OK;
    {
        AJ_Message msg;
        CLR_RT_HeapBlock * managedMsg = stack.Arg1().Dereference();  FAULT_ON_NULL(managedMsg);                
        
        CopyFromManagedMsg(managedMsg, &msg);
        
        CLR_RT_HeapBlock_String *  param1 = stack.Arg2().DereferenceString();

        UINT32 * param2;
        UINT8 heapblock2[CLR_RT_HEAP_BLOCK_SIZE];
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UINT32_ByRef( stack, heapblock2, 3, param2 ) );

        AJ_Status status = AJ_UnmarshalArgs(&msg, param1->StringText(), param2);
        
        CopyToManagedMsg(managedMsg, &msg);
        
        TINYCLR_CHECK_HRESULT( hr );
        SetResult_INT32( stack, status );

        TINYCLR_CHECK_HRESULT( Interop_Marshal_StoreRef( stack, heapblock2, 3 ) );
    }
    TINYCLR_NOCLEANUP();
}

HRESULT Library_spot_alljoyn_native_Microsoft_SPOT_AllJoyn_AJ::UnmarshalArgs___STRING__MicrosoftSPOTAllJoynAJMessage__STRING( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER(); hr = S_OK;
    {
        AJ_Message msg;
        CLR_RT_HeapBlock * managedMsg = stack.Arg1().Dereference();  FAULT_ON_NULL(managedMsg);                
        
        CopyFromManagedMsg(managedMsg, &msg);

        CLR_RT_HeapBlock_String *  sig = stack.Arg2().DereferenceString();
        
        const char* data;
        AJ_UnmarshalArgs(&msg, sig->StringText(), &data);
        
        CopyToManagedMsg(managedMsg, &msg);
        
        TINYCLR_CHECK_HRESULT( hr );
        SetResult_LPCSTR( stack, data );
    }
    TINYCLR_NOCLEANUP();
}

HRESULT Library_spot_alljoyn_native_Microsoft_SPOT_AllJoyn_AJ::BusReplyAcceptSession___MicrosoftSPOTAllJoynAJStatus__MicrosoftSPOTAllJoynAJMessage__U4( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER(); hr = S_OK;
    {
        AJ_Message msg;
        CLR_RT_HeapBlock * managedMsg = stack.Arg1().Dereference();  FAULT_ON_NULL(managedMsg);                
        
        CopyFromManagedMsg(managedMsg, &msg);

        UINT32 param1 = stack.Arg2().NumericByRef().u4;

        INT32 retVal = AJ_BusReplyAcceptSession(&msg, param1);
        
        CopyToManagedMsg(managedMsg, &msg);
        
        TINYCLR_CHECK_HRESULT( hr );
        SetResult_INT32( stack, retVal );

    }
    TINYCLR_NOCLEANUP();
}

HRESULT Library_spot_alljoyn_native_Microsoft_SPOT_AllJoyn_AJ::BusHandleBusMessageInner___MicrosoftSPOTAllJoynAJStatus__MicrosoftSPOTAllJoynAJMessage( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER(); hr = S_OK;
    {
        AJ_Message msg;
        CLR_RT_HeapBlock * managedMsg = stack.Arg1().Dereference();  FAULT_ON_NULL(managedMsg);                        
        CopyFromManagedMsg(managedMsg, &msg);
        
        INT32 retVal = AJ_BusHandleBusMessage(&msg);
        
        CopyToManagedMsg(managedMsg, &msg);
        TINYCLR_CHECK_HRESULT( hr );
        SetResult_INT32( stack, retVal );
    }
    TINYCLR_NOCLEANUP();
}

HRESULT Library_spot_alljoyn_native_Microsoft_SPOT_AllJoyn_AJ::CloseMsg___VOID__MicrosoftSPOTAllJoynAJMessage( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER(); hr = S_OK;
    {
        AJ_Message msg;
        CLR_RT_HeapBlock * managedMsg = stack.Arg1().Dereference();  FAULT_ON_NULL(managedMsg);                        
        CopyFromManagedMsg(managedMsg, &msg);
        
        AJ_CloseMsg(&msg);
        CopyToManagedMsg(managedMsg, &msg);
        TINYCLR_CHECK_HRESULT( hr );
    }
    TINYCLR_NOCLEANUP();
}

HRESULT Library_spot_alljoyn_native_Microsoft_SPOT_AllJoyn_AJ::Disconnect___VOID__U4( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER(); hr = S_OK;
    {
        UINT32 param0 = stack.Arg1().NumericByRef().u4;
        AJ_Disconnect((AJ_BusAttachment *)param0);
        TINYCLR_CHECK_HRESULT( hr );
    }
    TINYCLR_NOCLEANUP();
}

HRESULT Library_spot_alljoyn_native_Microsoft_SPOT_AllJoyn_AJ::Sleep___VOID__U4( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER(); hr = S_OK;
    {
        UINT32 param0 = stack.Arg1().NumericByRef().u4;
        AJ_Sleep(param0);
        TINYCLR_CHECK_HRESULT( hr );
    }
    TINYCLR_NOCLEANUP();
}

HRESULT Library_spot_alljoyn_native_Microsoft_SPOT_AllJoyn_AJ::AppMessageId___STATIC__U4__U4__U4__U4( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER(); hr = S_OK;
    {
        UINT32 param0 = stack.Arg0().NumericByRef().u4;
        UINT32 param1 = stack.Arg1().NumericByRef().u4;
        UINT32 param2 = stack.Arg2().NumericByRef().u4;

        UINT32 retVal = AJ_APP_MESSAGE_ID(param0, param1, param2);
        TINYCLR_CHECK_HRESULT( hr );
        SetResult_UINT32( stack, retVal );
    }
    TINYCLR_NOCLEANUP();
}

HRESULT Library_spot_alljoyn_native_Microsoft_SPOT_AllJoyn_AJ::BusMessageId___STATIC__U4__U4__U4__U4( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER(); hr = S_OK;
    {
        UINT32 param0 = stack.Arg0().NumericByRef().u4;
        UINT32 param1 = stack.Arg1().NumericByRef().u4;
        UINT32 param2 = stack.Arg2().NumericByRef().u4;

        UINT32 retVal = AJ_BUS_MESSAGE_ID(param0, param1, param2);
        TINYCLR_CHECK_HRESULT( hr );
        SetResult_UINT32( stack, retVal );

    }
    TINYCLR_NOCLEANUP();
}

HRESULT Library_spot_alljoyn_native_Microsoft_SPOT_AllJoyn_AJ::PrxMessageId___STATIC__U4__U4__U4__U4( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER(); hr = S_OK;
    {
        UINT32 param0 = stack.Arg0().NumericByRef().u4;
        UINT32 param1 = stack.Arg1().NumericByRef().u4;
        UINT32 param2 = stack.Arg2().NumericByRef().u4;

        UINT32 retVal = (param0, param1, param2);
        TINYCLR_CHECK_HRESULT( hr );
        SetResult_UINT32( stack, retVal );
    }
    TINYCLR_NOCLEANUP();
}

HRESULT Library_spot_alljoyn_native_Microsoft_SPOT_AllJoyn_AJ::AppPropertyId___STATIC__U4__U4__U4__U4( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER(); hr = S_OK;
    {
        UINT32 param0 = stack.Arg0().NumericByRef().u4;
        UINT32 param1 = stack.Arg1().NumericByRef().u4;
        UINT32 param2 = stack.Arg2().NumericByRef().u4;
        
        UINT32 retVal = AJ_APP_PROPERTY_ID(param0, param1, param2);
        TINYCLR_CHECK_HRESULT( hr );
        SetResult_UINT32( stack, retVal );
    }
    TINYCLR_NOCLEANUP();
}

HRESULT Library_spot_alljoyn_native_Microsoft_SPOT_AllJoyn_AJ::BusPropertyId___STATIC__U4__U4__U4__U4( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER(); hr = S_OK;
    {
        UINT32 param0 = stack.Arg0().NumericByRef().u4;
        UINT32 param1 = stack.Arg1().NumericByRef().u4;
        UINT32 param2 = stack.Arg2().NumericByRef().u4;

        UINT32 retVal = AJ_BUS_PROPERTY_ID(param0, param1, param2);
        TINYCLR_CHECK_HRESULT( hr );
        SetResult_UINT32( stack, retVal );
    }
    TINYCLR_NOCLEANUP();
}

HRESULT Library_spot_alljoyn_native_Microsoft_SPOT_AllJoyn_AJ::PrxPropertyId___STATIC__U4__U4__U4__U4( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER(); hr = S_OK;
    {
        UINT32 param0 = stack.Arg0().NumericByRef().u4;
        UINT32 param1 = stack.Arg1().NumericByRef().u4;
        UINT32 param2 = stack.Arg2().NumericByRef().u4;

        UINT32 retVal = AJ_PRX_PROPERTY_ID(param0, param1, param2);
        TINYCLR_CHECK_HRESULT( hr );
        SetResult_UINT32( stack, retVal );

    }
    TINYCLR_NOCLEANUP();
}

HRESULT Library_spot_alljoyn_native_Microsoft_SPOT_AllJoyn_AJ::CreateBus___VOID__BYREF_U4( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER(); hr = S_OK;
    {
        UINT32 * param0;
        UINT8 heapblock0[CLR_RT_HEAP_BLOCK_SIZE];
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UINT32_ByRef( stack, heapblock0, 1, param0 ) );

        *param0 = (UINT32)&BusInstance;
    
        TINYCLR_CHECK_HRESULT( hr );
        TINYCLR_CHECK_HRESULT( Interop_Marshal_StoreRef( stack, heapblock0, 1 ) );
    }
    TINYCLR_NOCLEANUP();
}

// FIXME not implemented
HRESULT Library_spot_alljoyn_native_Microsoft_SPOT_AllJoyn_AJ::PrintXML___STATIC__VOID__STRING__STRING__U1__I4( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER(); hr = S_OK;
    {
        TINYCLR_CHECK_HRESULT( hr );
    }
    TINYCLR_NOCLEANUP();
}
