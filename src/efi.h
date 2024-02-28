#include <stdint.h>
#include <stdbool.h>

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

typedef struct EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL;

typedef EFI_STATUS (EFIAPI* EFI_TEXT_RESET) (IN EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL* This, IN BOOLEAN ExtendedVerification);
typedef EFI_STATUS (EFIAPI* EFI_TEXT_STRING) (IN EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL* This, IN CHAR16* String);
typedef EFI_STATUS (EFIAPI* EFI_TEXT_CLEAR_SCREEN) (IN EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL* This);

typedef struct EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL
{
	EFI_TEXT_RESET Reset;
	EFI_TEXT_STRING OutputString;
	void* TestString;
	void* QueryMode;
	void* SetMode;
	void* SetAttribute;
	EFI_TEXT_CLEAR_SCREEN ClearScreen;
	void* SetCursorPosition;
	void* EnableCursor;
	void* Mode;
} EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL;

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
	CHAR16* FirmwareVendor;
	UINT32 FirmwareRevision;
	EFI_HANDLE ConsoleInHandle;
	void* ConIn;
	EFI_HANDLE ConsoleOutHandle;
	EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL* ConOut;
	EFI_HANDLE StandardErrorHandle;
	EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL* StdErr;
	void* RuntimeServices;
	void* BootServices;
	UINTN NumberOfTableEntries;
	void* ConfigurationTable;
} EFI_SYSTEM_TABLE;

typedef EFI_STATUS (EFIAPI* EFI_IMAGE_ENTRY_POINT) (IN EFI_HANDLE ImageHandle, IN EFI_SYSTEM_TABLE* SystemTable);
