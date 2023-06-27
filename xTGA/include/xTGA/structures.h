//============ Copyright © 2019 Brett Anthony. All rights reserved. ============
///
/// This work is licensed under the terms of the MIT license.
/// For a copy, see <https://opensource.org/licenses/MIT>.
//==============================================================================
/// @file structures.h
/// @brief Contains definitions for the header/footer/other structs that a TGA file contains.
//==============================================================================

#ifndef XTGA_STRUCTURES_H__
#define XTGA_STRUCTURES_H__

#include "xTGA/flags.h"
#include "xTGA/types.h"

#pragma pack(push, 1)

namespace xtga
{
	namespace structs
	{
		/**
		* @brief Contains various information related to a TGA file.
		*/
		struct ImageDescriptor
		{
			union
			{
				uchar RawBits;
				struct
				{
					uchar                    ALPHA_CHANNEL_BITCOUNT : 4; /*!< The number of bits dedicated to alpha/attribute per pixel. */
					xtga::flags::IMAGEORIGIN IMAGE_ORIGIN : 2;           /*!< The origin of the first pixel of the image, generates a warning on GCC, no way to disable :s */
					uchar                    UNUSED : 2;
				};
			};
		};

		/**
		* @brief Contains the most valuable information of the TGA File.
		*/
		struct Header
		{
			uchar            ID_LENGTH;                     /*!< Length of the Image ID Field (in bytes). */
			uchar            COLOR_MAP_TYPE;                /*!< 0 if there is no color map, 1 otherwise. */
			flags::IMAGETYPE IMAGE_TYPE;                    /*!< Image type. */
			uint16           COLOR_MAP_FIRST_ENTRY_INDEX;   /*!< Index of the first color map entry. */
			uint16           COLOR_MAP_LENGTH;              /*!< Total number of color map entries. */
			uchar            COLOR_MAP_BITS_PER_ENTRY;      /*!< Number of bits per color map entry. */
			uint16           ORIGIN_X;                      /*!< Absolute horizontal coordinate for the lower left corner of the image. */
			uint16           ORIGIN_Y;                      /*!< Absolute vertical coordinate for the lower left corner of the image. */
			uint16           IMAGE_WIDTH;                   /*!< Width of the image in pixels. */
			uint16           IMAGE_HEIGHT;                  /*!< Height of the image in pixels. */
			uchar            IMAGE_DEPTH;                   /*!< Bits per pixel of the image. */
			ImageDescriptor  IMAGE_DESCRIPTOR;              /*!< Contains the number of attribute bits per pixel/alpha bits per pixel, and image origin. */
		};

		/**
		* @brief Contains some offsets for the TGA File, the presence of this footer indicates that the TGA File is of version 2.0.
		*/
		struct Footer
		{
			uint32 EXTENSION_AREA_OFFSET;                   /*!< Offset for the extension area. */
			uint32 DEVELOPER_DIRECTORY_OFFSET;              /*!< Offset for the developer directory. */
			uchar	 SIGNATURE[18];                         /*!< Should always be "TRUEVISION-XFILE.\0" */
		};

		/**
		* @brief Simple data structure that describes an entry into our developer directory.
		*/
		struct DeveloperDirectoryEntry
		{
			uint16 TAG;                                     /*!< Identifies the type of developer entry. */
			uint32 ENTRY_OFFSET;                            /*!< The offset of the entry (from start of file). */
			uint32 ENTRY_SIZE;                              /*!< Size of the entry (in bytes). */
		};

		/**
		* @brief Contains extended metadata for the image.
		*/
		struct ExtensionArea
		{
			uint16 EXTENSION_SIZE;			        /*!< The size of the extension area. Should be 495 for TGA 2.0. */
			uchar                  AUTHOR_NAME[41];         /*!< The file author's name. Null-terminated. */
			uchar                  AUTHOR_COMMENTS_0[81];   /*!< 1st author comment line. Null-terminated. */
			uchar                  AUTHOR_COMMENTS_1[81];   /*!< 2nd author comment line. Null-terminated. */
			uchar                  AUTHOR_COMMENTS_2[81];   /*!< 3rd author comment line. Null-terminated. */
			uchar                  AUTHOR_COMMENTS_3[81];   /*!< 4th author comment line. Null-terminated. */
			uint16                 SAVE_DATE_MONTH;         /*!< The month the file was last saved/modified. */
			uint16                 SAVE_DATE_DAY;           /*!< The day the file was last saved/modified. */
			uint16                 SAVE_DATE_YEAR;          /*!< The year the file was last saved/modified. */
			uint16                 SAVE_DATE_HOUR;          /*!< The hour the file was last saved/modified. */
			uint16                 SAVE_DATE_MINUTE;        /*!< The minute the file was last saved/modified. */
			uint16                 SAVE_DATE_SECOND;        /*!< The second the file was last saved/modified. */
			uchar                  JOB_NAME[41];            /*!< Name/ID tag for the image. Null-terminated. */
			uint16                 JOB_HOURS;               /*!< The amount of billed hours for the image. */
			uint16                 JOB_MINUTES;             /*!< The amount of billed minutes for the image. */
			uint16                 JOB_SECONDS;             /*!< The number of billed seconds for the image. */
			uchar                  SOFTWARE_ID[41];         /*!< The software that was used to create the image. Null-terminated. */
			uint16                 SOFTWARE_VERSION;        /*!< The version of the software used to create the image. The version is multiplied by 100. i.e. for version 2.50, store 250. */
			uchar                  SOFTWARE_LETTER;         /*!< The letter that is added to the end of the software version, i.e. 'b' for beta. */
			pixelformats::BGRA8888 KEY_COLOR;               /*!< The chroma key color for the image in format BGRA. */
			uint16                 PIXEL_RATIO_X;           /*!< The numerator of the pixel aspect ratio. */
			uint16                 PIXEL_RATIO_Y;           /*!< The denominator of the pixel aspect ratio. Can be 0 to denote no pixel aspect ratio info. */
			uint16                 GAMMA_NUMERATOR;         /*!< The numerator of the gamma correction. */
			uint16                 GAMMA_DENOMINATOR;       /*!< The denominator of the gamma correction. Can be 0 to denote no gamma correction info. */
			uint32                 COLOR_CORRECTION_TABLE;  /*!< The offset from the beginning of the file for the color correction table. */
			uint32                 THUMBNAIL_OFFSET;        /*!< The offset from the beginning of the file for the thumbnail image. */
			uint32                 SCAN_LINE_OFFSET;        /*!< The offset from the beginning of the file for the scan line table. */
			xtga::flags::ALPHATYPE ALPHATYPE;               /*!< Details the type of alpha the image contains. */
		};

		/**
		* @brief simple data structure for what a run-length packet contains.
		*/
		struct RLEPacket
		{
			union
			{
				uchar RawBits;
				struct
				{
					uchar PIXEL_COUNT_MINUS_ONE : 7; /*!< The number of pixels the packet describes - 1 */
					uchar RUN_LENGTH : 1;            /*!< Whether or not the next pixel is a run-length pixel */
				};
			};
		};

		/**
		* @brief describes a simple color correction entry, this format has not been verified, if you are aware of the correct formats
		* please do contact me!
		*/
		struct ColorCorrectionEntry
		{
			uint16_t A;
			uint16_t R;
			uint16_t G;
			uint16_t B;

			ColorCorrectionEntry(const uint16_t& red, const uint16_t& green, const uint16_t& blue, const uint16_t& alpha = 65535)
			{
				A = alpha;
				R = red;
				G = green;
				B = blue;
			}
			ColorCorrectionEntry(const ColorCorrectionEntry& src) : A(src.A), R(src.R), G(src.G), B(src.B) { }
		};
	}
}

#pragma pack(pop)
#endif // !XTGA_STRUCTURES_H__
