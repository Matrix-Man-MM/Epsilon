#include <stdarg.h>
#include "efi.h"

EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL* cout = NULL;
void* cin = NULL;

void init_globals(EFI_SYSTEM_TABLE* sys_table)
{
	cout = sys_table->ConOut;
	cin = sys_table->ConIn;
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
	bool result = false;
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

EFI_STATUS efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable)
{
	(void)ImageHandle;
	(void)SystemTable;

	init_globals(SystemTable);

	cout->Reset(cout, false);
	cout->SetAttribute(cout, EFI_TEXT_ATTR(EFI_GREEN, EFI_BLACK));
	cout->ClearScreen(cout);

	printf(u"EPSILON UEFI BOOTLOADER\r\n");

	printf(u"Hex Test: %x\r\n", (UINTN)0x1234ABCD);
	printf(u"Negative Test: %d\r\n", (INT32)-12345);

	printf(u"Current Text Mode\r\n-------------------\r\n");
	UINTN max_cols = 0;
	UINTN max_rows = 0;

	cout->QueryMode(cout, cout->Mode->Mode, &max_cols, &max_rows);

	printf(u"Max Mode: %d\r\nCurrent Mode: %d\r\nAttribute: %x\r\nCursorColumn: %d\r\nCursorRow: %d\r\nCursorVisible: %d\r\nColumns: %d\r\nRows: %d\r\n", cout->Mode->MaxMode, cout->Mode->Mode, cout->Mode->Attribute, cout->Mode->CursorColumn, cout->Mode->CursorRow, cout->Mode->CursorVisible, max_cols, max_rows);

	while (1) ;
}
