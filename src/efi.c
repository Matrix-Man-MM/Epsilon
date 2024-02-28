#include "efi.h"

EFI_STATUS efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable)
{
	(void)ImageHandle;
	(void)SystemTable;

	SystemTable->ConOut->Reset(SystemTable->ConOut, false);

	SystemTable->ConOut->ClearScreen(SystemTable->ConOut);

	SystemTable->ConOut->OutputString(SystemTable->ConOut, u"EPSILON UEFI OPERATING SYSTEM\r\n");

	while (1) ;
}
