#include <stdarg.h>
#include "efi.h"

#define ARRAY_SIZE(x) (sizeof(x) / sizeof(x)[0])

#define SCANCODE_ESC 0x17
#define SCANCODE_UPARROW 0x1
#define SCANCODE_DOWNARROW 0x2

#define DEFAULT_FG_COLOR EFI_GREEN
#define DEFAULT_BG_COLOR EFI_BLACK

#define HIGHLIGHT_FG_COLOR EFI_CYAN
#define HIGHLIGHT_BG_COLOR EFI_BLUE

EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL* cout = NULL;
EFI_SIMPLE_TEXT_INPUT_PROTOCOL* cin = NULL;
EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL* cerr = NULL;
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

EFI_STATUS settextmode(void)
{
	while (true)
	{
		cout->ClearScreen(cout);
		printf(u"Text Mode Info\r\n----------------------\r\n");
		
		UINTN max_rows = 0;
		UINTN max_cols = 0;

		cout->QueryMode(cout, cout->Mode->Mode, &max_cols, &max_rows);

		printf(u"Max Mode: %d\r\nCurrent Mode: %d\r\nAttribute: %x\r\nCursorColumn: %d\r\nCursorRow: %d\r\nCursorVisible: %d\r\nColumns: %d\r\nRows: %d\r\n", cout->Mode->MaxMode, cout->Mode->Mode, cout->Mode->Attribute, cout->Mode->CursorColumn, cout->Mode->CursorRow, cout->Mode->CursorVisible, max_cols, max_rows);

		printf(u"Available Text Modes\r\n--------------------------\r\n");

		const INT32 max = cout->Mode->MaxMode;
		for (INT32 i = 0; i < max; i++)
		{
			cout->QueryMode(cout, i, &max_cols, &max_rows);
			printf(u"Mode #: %d, %dx%d\r\n", i, max_cols, max_rows);
		}

		while (true)
		{
			static UINTN current_mode = 0;
			current_mode = cout->Mode->Mode;

			for (UINTN i = 0; i < 79; i++)
				printf(u" ");

			printf(u"\r\nSelect Text Mode (0-%d): %d", max, current_mode);

			cout->SetCursorPosition(cout, cout->Mode->CursorColumn-1, cout->Mode->CursorRow);

			EFI_INPUT_KEY key = getkey();

			CHAR16 char_buf[2];
			char_buf[0] = key.UnicodeChar;
			char_buf[1] = u'\0';

			printf(u"%s ", char_buf);

			if (key.ScanCode == SCANCODE_ESC)
				return EFI_SUCCESS;

			current_mode = key.UnicodeChar - u'0';
			EFI_STATUS status = cout->SetMode(cout, current_mode);

			if (EFI_ERROR(status))
			{
				if (status == EFI_DEVICE_ERROR)
					printf(u"ERROR: %x; DEVICE ERROR", status);
				else if (status == EFI_UNSUPPORTED)
					printf(u"ERROR: %x; MODE # IS INVALID", status);

				printf(u"\r\nPress any key to select again...", status);
				getkey();
			}

			break;
		}
	}

	return EFI_SUCCESS;
}

EFI_STATUS setgraphicsmode(void)
{
	return EFI_SUCCESS;
}

EFI_STATUS efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable)
{
	init_globals(ImageHandle, SystemTable);

	cout->Reset(cout, false);
	cout->SetAttribute(cout, EFI_TEXT_ATTR(EFI_GREEN, EFI_BLACK));

	bool is_running = true;
	while (is_running)
	{
		const CHAR16* menu_choices[] =
		{
			u"Set Text Mode",
			u"Set Graphics Mode",
		};

		EFI_STATUS (*menu_functions[])(void) =
		{
			settextmode,
			setgraphicsmode,
		};

		cout->ClearScreen(cout);

		UINTN cols = 0;
		UINTN rows = 0;
		cout->QueryMode(cout, cout->Mode->Mode, &cols, &rows);

		cout->SetCursorPosition(cout, 0, rows-3);
		printf(u"Up/Down Arrow: Move Cursor\r\nEnter: Select\r\nEscape: Shut Down");

		cout->SetCursorPosition(cout, 0, 0);
		cout->SetAttribute(cout, EFI_TEXT_ATTR(HIGHLIGHT_FG_COLOR, HIGHLIGHT_BG_COLOR));
		printf(u"%s", menu_choices[0]);

		cout->SetAttribute(cout, EFI_TEXT_ATTR(DEFAULT_FG_COLOR, DEFAULT_BG_COLOR));
		for (UINTN i = 1; i < ARRAY_SIZE(menu_choices); i++)
			printf(u"\r\n%s", menu_choices[i]);

		INTN min_row = 0;
		INTN max_row = cout->Mode->CursorRow;

		cout->SetCursorPosition(cout, 0, 0);
		bool is_getting_input = true;
		while (is_getting_input)
		{
			INTN current_row = cout->Mode->CursorRow;
			EFI_INPUT_KEY key = getkey();

			switch (key.ScanCode)
			{
				case SCANCODE_UPARROW:
					if (current_row - 1 >= min_row)
					{
						cout->SetAttribute(cout, EFI_TEXT_ATTR(DEFAULT_FG_COLOR, DEFAULT_BG_COLOR));
						printf(u"%s\r\n", menu_choices[current_row]);

	
						current_row--;
						cout->SetCursorPosition(cout, 0, current_row);
						cout->SetAttribute(cout, EFI_TEXT_ATTR(HIGHLIGHT_FG_COLOR, HIGHLIGHT_BG_COLOR));
						printf(u"%s\r\n", menu_choices[current_row]);

						cout->SetAttribute(cout, EFI_TEXT_ATTR(DEFAULT_FG_COLOR, DEFAULT_BG_COLOR));
					}
					break;

				case SCANCODE_DOWNARROW:
					if (current_row+1 <= max_row)
					{
						cout->SetAttribute(cout, EFI_TEXT_ATTR(DEFAULT_FG_COLOR, DEFAULT_BG_COLOR));
						printf(u"%s\r\n", menu_choices[current_row]);

						current_row++;
						cout->SetCursorPosition(cout, 0, current_row);
						cout->SetAttribute(cout, EFI_TEXT_ATTR(HIGHLIGHT_FG_COLOR, HIGHLIGHT_BG_COLOR));
						printf(u"%s\r\n", menu_choices[current_row]);

						cout->SetAttribute(cout, EFI_TEXT_ATTR(DEFAULT_FG_COLOR, DEFAULT_BG_COLOR));
					}
					break;

				case SCANCODE_ESC:
					rs->ResetSystem(EfiResetShutdown, EFI_SUCCESS, 0, NULL);
					break;

				default:
					if (key.UnicodeChar == u'\r')
					{
						EFI_STATUS return_stat = menu_functions[current_row]();

						if (EFI_ERROR(return_stat))
						{
							printf(u"ERROR %x\r\n; Press Any Key To Go Back...", return_stat);
							getkey();
						}

						is_getting_input = false;
					}
					break;
			}
		}
	}

	return EFI_SUCCESS;
}
