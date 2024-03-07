#include <stdarg.h>
#include "efi.h"

#define SCANCODE_ESC 0x17

EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL* cout = NULL;
EFI_SIMPLE_TEXT_INPUT_PROTOCOL* cin = NULL;
EFI_BOOT_SERVICES* bs;
EFI_RUNTIME_SERVICES* rs;
EFI_HANDLE image = NULL;

void init_globals(EFI_HANDLE handle, EFI_SYSTEM_TABLE* sys_table)
{
	cout = sys_table->ConOut;
	cin = sys_table->ConIn;
	bs = sys_table->BootServices;
	rs = sys_table->RuntimeServices;
	image = handle;
}

bool putint(INT32 n)
{
	const CHAR16* digits = u"0123456789";
	CHAR16 buf[11];
	UINTN i = 0;
	const bool is_negative = (n < 0);

	if (is_negative)
		n = -n;

	do
	{
		buf[i++] = digits[n % 10];
		n /= 10;
	} while (n > 0);

	if (is_negative)
		buf[i++] = u'-';

	buf[i--] = u'\0';

	for (UINTN j = 0; j < i; j++, i--)
	{
		UINTN tmp = buf[i];
		buf[i] = buf[j];
		buf[j] = tmp;
	}

	cout->OutputString(cout, buf);

	return true;
}

bool puthex(UINTN n)
{
	const CHAR16* digits = u"0123456789ABCDEF";
	CHAR16 buf[20];
	UINTN i = 0;

	do
	{
		buf[i++] = digits[n % 16];
		n /= 16;
	} while (n > 0);

	buf[i++] = u'x';
	buf[i++] = u'0';

	buf[i--] = u'\0';

	for (UINTN j = 0; j < i; j++, i--)
	{
		UINTN tmp = buf[i];
		buf[i] = buf[j];
		buf[j] = tmp;
	}

	cout->OutputString(cout, buf);

	return true;
}

bool printf(CHAR16* fmt, ...)
{
	bool result = true;
	CHAR16 charstr[2];
	va_list args;

	va_start(args, fmt);

	charstr[0] = u'\0';
	charstr[1] = u'\0';

	for (UINTN i = 0; fmt[i] != u'\0'; i++)
	{
		if (fmt[i] == u'%')
		{
			i++;

			switch (fmt[i])
			{
				case u's':
					CHAR16* str = va_arg(args, CHAR16*);
					cout->OutputString(cout, str);
					break;
				case u'd':
					INT32 num = va_arg(args, INT32);
					putint(num);
					break;
				case u'x':
					UINTN number = va_arg(args, UINTN);
					puthex(number);
					break;
				default:
					cout->OutputString(cout, u"Invalid Format: %");
					charstr[0] = fmt[i];
					cout->OutputString(cout, charstr);
					cout->OutputString(cout, u"\r\n");
					result = false;
					goto end;
					break;
			}
		}
		else
		{
			charstr[0] = fmt[i];
			cout->OutputString(cout, charstr);
		}
	}

end:
	va_end(args);

	result = true;
	return result;
}

EFI_INPUT_KEY getkey(void)
{
	EFI_EVENT events[1];
	EFI_INPUT_KEY key;

	key.ScanCode = 0;
	key.UnicodeChar = u'\0';

	events[0] = cin->WaitForKey;
	UINTN index = 0;
	bs->WaitForEvent(1, events, &index);

	if (index == 0)
		cin->ReadKeyStroke(cin, &key);

	return key;
}

EFI_STATUS efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable)
{
	init_globals(ImageHandle, SystemTable);

	cout->Reset(cout, false);
	cout->SetAttribute(cout, EFI_TEXT_ATTR(EFI_GREEN, EFI_BLACK));

	bool is_running = true;
	while (is_running)
	{
		cout->ClearScreen(cout);

		printf(u"Text Mode Info\r\n-------------------\r\n");
		UINTN max_cols = 0;
		UINTN max_rows = 0;
		cout->QueryMode(cout, cout->Mode->Mode, &max_cols, &max_rows);
		printf(u"Max Mode: %d\r\nCurrent Mode: %d\r\nAttribute: %x\r\nCursorColumn: %d\r\nCursorRow: %d\r\nCursorVisible: %d\r\nColumns: %d\r\nRows: %d\r\n\r\n", cout->Mode->MaxMode, cout->Mode->Mode, cout->Mode->Attribute, cout->Mode->CursorColumn, cout->Mode->CursorRow, cout->Mode->CursorVisible, max_cols, max_rows);

		printf(u"Available Text Modes\r\n--------------------------\r\n");

		const INT32 max = cout->Mode->MaxMode;
		for (INT32 i = 0; i < max; i++)
		{
			cout->QueryMode(cout, i, &max_cols, &max_rows);
			printf(u"Mode #: %d, %dx%d\r\n", i, max_cols, max_rows);
		}

		while (1)
		{
			static UINTN current_mode = 0;
			current_mode = cout->Mode->Mode;

			for (UINTN i = 0; i < 79; i++)
				printf(u" ");

			printf(u"\rSelect Text Mode # (0-%d): %d", max, current_mode);

			cout->SetCursorPosition(cout, cout->Mode->CursorColumn-1, cout->Mode->CursorRow);

			EFI_INPUT_KEY key = getkey();

			CHAR16 cbuf[2];
			cbuf[0] = key.UnicodeChar;
			cbuf[1] = u'\0';

			printf(u"%s ", cbuf);

			if (key.ScanCode == SCANCODE_ESC)
				rs->ResetSystem(EfiResetShutdown, EFI_SUCCESS, 0, NULL);

			current_mode = key.UnicodeChar - u'0';
			EFI_STATUS status = cout->SetMode(cout, current_mode);

			if (EFI_ERROR(status))
			{
				if (status == EFI_DEVICE_ERROR)
					printf(u"ERROR: %x; Device Error\r\n", status);
				else if (status == EFI_UNSUPPORTED)
					printf(u"ERROR: %x; Mode # Invalid\r\n", status);

				getkey();
			}
			else
				break;
		}
	}

	return EFI_SUCCESS;
}
