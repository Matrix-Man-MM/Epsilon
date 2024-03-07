#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#if __has_include(<uchar.h>)
#include <uchar.h>
#endif

#define IN
#define OUT
#define OPTIONAL
#define CONST const

#define EFIAPI __attribute__((ms_abi))

typedef uint8_t BOOLEAN;
typedef int64_t INTN;
typedef uint64_t UINTN;
typedef int8_t INT8;
typedef uint8_t UINT8;
typedef int16_t INT16;
typedef uint16_t UINT16;
typedef int32_t INT32;
typedef uint32_t UINT32;
typedef int64_t INT64;
typedef uint64_t UINT64;
typedef char CHAR8;

#ifndef _UCHAR_H
typedef uint_least16_t char16_t;
#endif

typedef char16_t CHAR16;

typedef void VOID;

typedef struct
{
	UINT32 TimeLow;
	UINT16 TimeMid;
	UINT16 TimeHighAndVersion;
	UINT8 ClockSeqHighAndReserved;
	UINT8 ClockSeqLow;
	UINT8 Node[6];
} __attribute__ ((packed)) EFI_GUID;

typedef UINTN EFI_STATUS;
typedef VOID* EFI_HANDLE;
typedef VOID* EFI_EVENT;
typedef UINT64 EFI_LBA;
typedef UINTN EFI_TPL;

#define EFI_SUCCESS 0ULL

#define TOP_BIT 0x8000000000000000
#define ENCODE_ERROR(x) (TOP_BIT | (x))
#define EFI_ERROR(x) ((INTN)((UINTN)(x)) < 0)

#define EFI_UNSUPPORTED ENCODE_ERROR(3)
#define EFI_DEVICE_ERROR ENCODE_ERROR(7)

typedef struct EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL;

typedef EFI_STATUS (EFIAPI* EFI_TEXT_RESET) (IN EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL* This, IN BOOLEAN ExtendedVerification);
typedef EFI_STATUS (EFIAPI* EFI_TEXT_STRING) (IN EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL* This, IN CHAR16* String);
typedef EFI_STATUS (EFIAPI* EFI_TEXT_QUERY_MODE) (IN EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL* This, IN UINTN ModeNumber, OUT UINTN* Columns, OUT UINTN* Rows);
typedef EFI_STATUS (EFIAPI* EFI_TEXT_SET_MODE) (IN EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL* This, IN UINTN ModeNumber);
typedef EFI_STATUS (EFIAPI* EFI_TEXT_SET_ATTRIBUTE) (IN EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL* This, IN UINTN Attribute);

#define EFI_BLACK 0x00
#define EFI_BLUE 0x01
#define EFI_GREEN 0x02
#define EFI_CYAN 0x03
#define EFI_RED 0x04
#define EFI_MAGENTA 0x05
#define EFI_BROWN 0x06
#define EFI_LIGHTGRAY 0x07
#define EFI_BRIGHT 0x08
#define EFI_DARKGRAY 0x08
#define EFI_LIGHTBLUE 0x09
#define EFI_LIGHTGREEN 0x0A
#define EFI_LIGHTCYAN 0x0B
#define EFI_LIGHTRED 0x0C
#define EFI_LIGHTMAGENTA 0x0D
#define EFI_YELLOW 0x0E
#define EFI_WHITE 0x0F

#define EFI_BACKGROUND_BLACK 0x00
#define EFI_BACKGROUND_BLUE 0x10
#define EFI_BACKGROUND_GREEN 0x20
#define EFI_BACKGROUND_CYAN 0x30
#define EFI_BACKGROUND_RED 0x40
#define EFI_BACKGROUND_MAGENTA 0x50
#define EFI_BACKGROUND_BROWN 0x60
#define EFI_BACKGROUND_LIGHTGRAY 0x70

#define EFI_TEXT_ATTR(Foreground, Background) ((Foreground) | ((Background) << 4))

typedef EFI_STATUS (EFIAPI* EFI_TEXT_CLEAR_SCREEN) (IN EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL* This);
typedef EFI_STATUS (EFIAPI* EFI_TEXT_SET_CURSOR_POSITION) (IN EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL* This, IN UINTN Column, IN UINTN Row);

typedef struct
{
	INT32 MaxMode;
	INT32 Mode;
	INT32 Attribute;
	INT32 CursorColumn;
	INT32 CursorRow;
	BOOLEAN CursorVisible;
} SIMPLE_TEXT_OUTPUT_MODE;

typedef struct EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL
{
	EFI_TEXT_RESET Reset;
	EFI_TEXT_STRING OutputString;
	void* TestString;
	EFI_TEXT_QUERY_MODE QueryMode;
	EFI_TEXT_SET_MODE SetMode;
	EFI_TEXT_SET_ATTRIBUTE SetAttribute;
	EFI_TEXT_CLEAR_SCREEN ClearScreen;
	EFI_TEXT_SET_CURSOR_POSITION SetCursorPosition;
	void* EnableCursor;
	SIMPLE_TEXT_OUTPUT_MODE* Mode;
} EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL;

typedef struct EFI_SIMPLE_TEXT_INPUT_PROTOCOL EFI_SIMPLE_TEXT_INPUT_PROTOCOL;

typedef EFI_STATUS (EFIAPI* EFI_INPUT_RESET) (IN EFI_SIMPLE_TEXT_INPUT_PROTOCOL* This, IN BOOLEAN ExtendedVerification);

typedef struct
{
	UINT16 ScanCode;
	CHAR16 UnicodeChar;
} EFI_INPUT_KEY;

typedef EFI_STATUS (EFIAPI* EFI_INPUT_READ_KEY) (IN EFI_SIMPLE_TEXT_INPUT_PROTOCOL* This, OUT EFI_INPUT_KEY* Key);

typedef struct EFI_SIMPLE_TEXT_INPUT_PROTOCOL
{
	EFI_INPUT_RESET Reset;
	EFI_INPUT_READ_KEY ReadKeyStroke;
	EFI_EVENT WaitForKey;
} EFI_SIMPLE_TEXT_INPUT_PROTOCOL;

typedef EFI_STATUS (EFIAPI* EFI_WAIT_FOR_EVENT) (IN UINTN NumberOfEvents, IN EFI_EVENT* Event, OUT UINTN* Index);

typedef enum
{
	EfiResetCold,
	EfiResetWarm,
	EfiResetShutdown,
	EfiResetPlatformSpecific
} EFI_RESET_TYPE;

typedef VOID (EFIAPI* EFI_RESET_SYSTEM) (IN EFI_RESET_TYPE ResetType, IN EFI_STATUS ResetStatus, IN UINTN DataSize, IN VOID* ResetData OPTIONAL);

typedef struct
{
	UINT64 Signature;
	UINT32 Revision;
	UINT32 HeaderSize;
	UINT32 CRC32;
	UINT32 Reserved;
} EFI_TABLE_HEADER;

typedef struct
{
	EFI_TABLE_HEADER Hdr;
	void* GetTime;
	void* SetTime;
	void* GetWakeupTime;
	void* SetWakeupTime;
	void* SetVirtualAddressMap;
	void* ConvertPointer;
	void* GetVariable;
	void* GetNextVariableName;
	void* SetVariable;
	void* GetNextHighMonotonicCount;
	EFI_RESET_SYSTEM ResetSystem;
	void* UpdateCapsule;
	void* QueryCapsuleCapabilities;
	void* QueryVariableInfo;
} EFI_RUNTIME_SERVICES;

typedef struct
{
	EFI_TABLE_HEADER Hdr;
	void* RaiseTPL;
	void* RestoreTPL;
	void* AllocatePages;
	void* FreePages;
	void* GetMemoryMap;
	void* AllocatePool;
	void* FreePool;
	void* CreateEvent;
	void* SetTimer;
	EFI_WAIT_FOR_EVENT WaitForEvent;
	void* SignalEvent;
	void* CloseEvent;
	void* CheckEvent;
	void* InstallProtocolInterface;
	void* ReinstallProtocolInterface;
	void* UninstallProtocolInterface;
	void* HandleProtocol;
	void* Reserved;
	void* RegisterProtocolNotify;
	void* LocateHandle;
	void* LocateDevicePath;
	void* InstallConfigurationTable;
	void* LoadImage;
	void* StartImage;
	void* Exit;
	void* UnloadImage;
	void* ExitBootServices;
	void* GetNextMonotonicCount;
	void* Stall;
	void* SetWatchdogTimer;
	void* ConnectController;
	void* DisconnectController;
	void* OpenProtocol;
	void* CloseProtocol;
	void* OpenProtocolInformation;
	void* ProtocolsPerHandle;
	void* LocateHandleBuffer;
	void* LocateProtocol;
	void* InstallMultipleProtocolInterfaces;
	void* UninstallMultipleProtocolInterfaces;
	void* CalculateCrc32;
	void* CopyMem;
	void* SetMem;
	void* CreateEventEx;
} EFI_BOOT_SERVICES;

typedef struct
{
	EFI_TABLE_HEADER Hdr;
	CHAR16* FirmwareVendor;
	UINT32 FirmwareRevision;
	EFI_HANDLE ConsoleInHandle;
	EFI_SIMPLE_TEXT_INPUT_PROTOCOL* ConIn;
	EFI_HANDLE ConsoleOutHandle;
	EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL* ConOut;
	EFI_HANDLE StandardErrorHandle;
	EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL* StdErr;
	EFI_RUNTIME_SERVICES* RuntimeServices;
	EFI_BOOT_SERVICES* BootServices;
	UINTN NumberOfTableEntries;
	void* ConfigurationTable;
} EFI_SYSTEM_TABLE;

typedef EFI_STATUS (EFIAPI* EFI_IMAGE_ENTRY_POINT) (IN EFI_HANDLE ImageHandle, IN EFI_SYSTEM_TABLE* SystemTable);
