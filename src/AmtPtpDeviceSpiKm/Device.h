/*++

Module Name:

    device.h

Abstract:

    This file contains the device definitions.

Environment:

    Kernel-mode Driver Framework

--*/

#include "public.h"

EXTERN_C_START

typedef struct _SPI_TRACKPAD_INFO {
	USHORT VendorId;
	USHORT ProductId;
	SHORT XMin;
	SHORT XMax;
	SHORT YMin;
	SHORT YMax;
} SPI_TRACKPAD_INFO, *PSPI_TRACKPAD_INFO;

typedef enum _REPORT_TYPE {
	PrecisionTouchpad = 0,
	Touchscreen = 1,
	InvalidReportType = 0x7fffffff,
} REPORT_TYPE;

//
// The device context performs the same job as
// a WDM device extension in the driver frameworks
//
typedef struct _DEVICE_CONTEXT
{
	// IO content
	WDFDEVICE	SpiDevice;
	WDFIOTARGET SpiTrackpadIoTarget;
	WDFQUEUE	HidIoQueue;
	BOOLEAN		DeviceReady;
	HANDLE		InputPollThreadHandle;

	// SPI device metadata
	USHORT HidVendorID;
	USHORT HidProductID;
	USHORT HidVersionNumber;
	SPI_TRACKPAD_INFO TrackpadInfo;
	REPORT_TYPE ReportType;

	// Windows PTP context
	BOOLEAN PtpInputOn;
	BOOLEAN PtpReportTouch;
	BOOLEAN PtpReportButton;

	// Timer
	LARGE_INTEGER LastReportTime;

	// Asynchronous & Reuse content
	KEVENT PtpRequestRoutineEvent;
	KEVENT PtpLoopRoutineEvent;
	BOOLEAN DelayedRequest;
	BOOLEAN PendingRequest;

} DEVICE_CONTEXT, *PDEVICE_CONTEXT;

//
// This macro will generate an inline function called DeviceGetContext
// which will be used to get a pointer to the device context memory
// in a type safe manner.
//
WDF_DECLARE_CONTEXT_TYPE_WITH_NAME(DEVICE_CONTEXT, DeviceGetContext)

//
// Function to initialize the device and its callbacks
//
NTSTATUS
AmtPtpDeviceSpiKmCreateDevice(
    _Inout_ PWDFDEVICE_INIT DeviceInit
    );

_IRQL_requires_(PASSIVE_LEVEL)
NTSTATUS
AmtPtpEvtDevicePrepareHardware(
	_In_ WDFDEVICE Device,
	_In_ WDFCMRESLIST ResourceList,
	_In_ WDFCMRESLIST ResourceListTranslated
);

_IRQL_requires_(PASSIVE_LEVEL)
NTSTATUS
AmtPtpEvtDeviceD0Entry(
	_In_ WDFDEVICE Device,
	_In_ WDF_POWER_DEVICE_STATE PreviousState
);

_IRQL_requires_(PASSIVE_LEVEL)
NTSTATUS
AmtPtpEvtDeviceD0Exit(
	_In_ WDFDEVICE Device,
	_In_ WDF_POWER_DEVICE_STATE TargetState
);

_IRQL_requires_(PASSIVE_LEVEL)
PCHAR
DbgDevicePowerString(
	_In_ WDF_POWER_DEVICE_STATE Type
);

_IRQL_requires_(PASSIVE_LEVEL)
NTSTATUS
AmtPtpSpiSetState(
	_In_ WDFDEVICE Device,
	_In_ BOOLEAN DesiredState
);

EXTERN_C_END
