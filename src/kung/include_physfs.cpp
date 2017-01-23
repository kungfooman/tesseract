#include <stdio.h>
#include "include_physfs.h"
#include <stdarg.h>


/**
 * @brief      fgets() implemented using PHYSFS.
 * @param      s Pointer to an array where the read chars will be stored.
 * @param      size Maximum number of chars to read (includes terminating null character).
 * @param      stream PHYSFS file handle.
 * @return     s on success, NULL on error or if no characters were read.
 * @note       PHYSFS_getLastError() or PHYSFS_eof() can help find the source
 *                     of the error.
 * @note       If a EOF is encountered before any chars are read, the chars
 *                     pointed by s are not changed.
 *
 * This function reads in at most size - 1 characters from stream
 * and stores them into the buffer pointed to by s.
 * Reading stops after an EOF or a newline and a null char is automatically appended.
 * If a newline is read, it is stored into the buffer.
 */
char *PHYSFS_fgets(char *s, int size, PHYSFS_file *stream) {
	char c;
	int i = 0;
	PHYSFS_sint64 retval;

	if (size <= 0 || !stream || !s || PHYSFS_eof(stream))
	{
		return NULL;
	}
	do
	{
		retval = PHYSFS_read(stream, &c, 1, 1);

		if (retval < 1)
		{
			break;
		}
		if (c == '\r')
			continue;
		s[i++] = c;
	}
	while (c != '\n' && i < size - 1);
	s[i] = '\0';

	// Success conditions
	if (retval == 1 // Read maximum chars or newline
	    || (i != 0 && PHYSFS_eof(stream) != 0)) /* Read something and stopped at EOF
	                                          * (note: i!=0 *should* be redundant) */
	{
		return s;
	}

	// Complete failure
	return NULL;
}

int PHYSFS_fprintf(PHYSFS_file *f, char *format, ...) {
	char buf[4096];
	va_list args;
	va_start (args, format);
	int bytes_written = vsprintf(buf, format, args);
	va_end (args);
	PHYSFS_write(f, buf, 1, bytes_written);
	return bytes_written;
}