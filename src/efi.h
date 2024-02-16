#if __has_include(<uchar.h>)
#include <uchar.h>
#endif

#include <stdint.h>
#include <stddef.h>

#ifndef _UCHAR_H
typedef uint_least16_t char16_t;
#endif

typedef uint16_t UINT16;
typedef uint32_t UINT32;
typedef uint64_t UINT64;
typedef uint64_t UINTN;
typedef char16_t CHAR16;
typedef void VOID;

typedef UINTN EFI_STATUS;
typedef VOID* EFI_HANDLE;

#define IN
#define OUT
#define OPTIONAL
#define CONST const

#define EFIAPI __attribute__((ms_abi))

#define EFI_SUCCESS 0

typedef struct EFI_SIMPLE_TEXT_INPUT_PROTOCOL EFI_SIMPLE_TEXT_INPUT_PROTOCOL;

typedef struct
{
	UINT16 ScanCode;
	CHAR16 UnicodeChar;
} EFI_INPUT_KEY;

typedef EFI_STATUS (EFIAPI* EFI_INPUT_READ_KEY) (IN EFI_SIMPLE_TEXT_INPUT_PROTOCOL* This, OUT EFI_INPUT_KEY* Key);

typedef struct EFI_SIMPLE_TEXT_INPUT_PROTOCOL
{
	void* Reset;
	EFI_INPUT_READ_KEY ReadKeyStroke;
	void* WaitForKey;
} EFI_SIMPLE_TEXT_INPUT_PROTOCOL;

typedef struct EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL;

#define EFI_BLACK 0x00
#define EFI_BLUE 0x01
#define EFI_GREEN 0x02
#define EFI_CYAN 0x03
#define EFI_RED 0x04
#define EFI_YELLOW 0x0E
#define EFI_WHITE 0x0F

#define EFI_TEXT_ATTR(Foreground, Background) ((Foreground) | ((Background) << 4))

typedef EFI_STATUS (EFIAPI* EFI_TEXT_STRING) (IN EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL* This, IN CHAR16* String);

typedef EFI_STATUS (EFIAPI* EFI_TEXT_SET_ATTRIBUTE) (IN EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL* This, IN UINTN Attribute);

typedef EFI_STATUS (EFIAPI* EFI_TEXT_CLEAR_SCREEN) (IN EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL* This);

typedef struct EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL
{
	void* Reset;
	EFI_TEXT_STRING OutputString;
	void* TestString;
	void* QueryMode;
	void* SetMode;
	EFI_TEXT_SET_ATTRIBUTE SetAttribute;
	EFI_TEXT_CLEAR_SCREEN ClearScreen;
	void* SetCursorPosition;
	void* EnableCursor;
	void* Mode;
} EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL;

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

	void* FirmwareVendor;
	UINT32 FirmwareRevision;
	void* ConsoleInHandle;
	EFI_SIMPLE_TEXT_INPUT_PROTOCOL* ConIn;
	void* ConsoleOutHandle;
	EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL* ConOut;
	void* StandardErrorHandle;
	void* StdErr;
	EFI_RUNTIME_SERVICES* RuntimeServices;
	void* BootServices;
	UINTN NumberOfTableEntries;
	void* ConfigurationTable;
} EFI_SYSTEM_TABLE;
