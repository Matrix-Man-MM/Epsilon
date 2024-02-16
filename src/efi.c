#include "efi.h"

EFI_STATUS EFIAPI efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable)
{
	(void)ImageHandle;

	SystemTable->ConOut->SetAttribute(SystemTable->ConOut, EFI_TEXT_ATTR(EFI_WHITE, EFI_BLUE));

	SystemTable->ConOut->ClearScreen(SystemTable->ConOut);

	SystemTable->ConOut->OutputString(SystemTable->ConOut, u"Welcome to Epsilon!\r\n");
	SystemTable->ConOut->OutputString(SystemTable->ConOut, u"Press any key to shutdown...");

	EFI_INPUT_KEY key;
	while (SystemTable->ConIn->ReadKeyStroke(SystemTable->ConIn, &key) != EFI_SUCCESS) ;

	SystemTable->RuntimeServices->ResetSystem(EfiResetShutdown, EFI_SUCCESS, 0, NULL);

	return EFI_SUCCESS;
}
