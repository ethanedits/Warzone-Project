#include "renderer.h"


namespace renderer {
	namespace fonts {
		IDWriteTextFormat* watermark_font = nullptr;
		IDWriteTextFormat* tahoma_font = nullptr;
		IDWriteTextFormat* header_tahoma_font = nullptr;
		IDWriteTextFormat* futura_font = nullptr;

		bool init( ) {

			if (FAILED(internal::factory->CreateTextFormat(L"Futura MdCn BT", NULL, DWRITE_FONT_WEIGHT_REGULAR, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 13, L"en-us", &futura_font))) {
				return false;
			}

			if (FAILED(internal::factory->CreateTextFormat(L"Tahoma", NULL, DWRITE_FONT_WEIGHT_REGULAR, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 22, L"en-us", &watermark_font))) {
				return false;
			}
			
			if (FAILED(internal::factory->CreateTextFormat(L"Tahoma", NULL, DWRITE_FONT_WEIGHT_REGULAR, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 13.0, L"en-us", &tahoma_font))) {
				return false;
			}

			if (FAILED(internal::factory->CreateTextFormat(L"Tahoma", NULL, DWRITE_FONT_WEIGHT_REGULAR, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 16.0, L"en-us", &header_tahoma_font))) {
				return false;
			}

			return true;
		}

		void shutdown( ) {
			watermark_font->Release( );
			tahoma_font->Release( );
			futura_font->Release();
			header_tahoma_font->Release();
		}
	}
}