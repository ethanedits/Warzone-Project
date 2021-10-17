#pragma once
#include "internal.h"
#include "../../utils/vectors.h"

namespace renderer {
	namespace colors {
		extern ID2D1SolidColorBrush* white_color;
		extern ID2D1SolidColorBrush* red_color;
		extern ID2D1SolidColorBrush* darkred_color;
		extern ID2D1SolidColorBrush* green_color;
		extern ID2D1SolidColorBrush* blue_color;
		extern ID2D1SolidColorBrush* black_color;
		extern ID2D1SolidColorBrush* cyan_color;
		extern ID2D1SolidColorBrush* orange_color;
		extern ID2D1SolidColorBrush* gray_color;
		extern ID2D1SolidColorBrush* dark_gray_color;
		extern ID2D1SolidColorBrush* dark_gray_color2;
		extern ID2D1SolidColorBrush* light_gray_color;
		extern ID2D1SolidColorBrush* light_gray_color2;
		extern ID2D1SolidColorBrush* light_gray_color3;
		extern ID2D1SolidColorBrush* light_gray_color4;

		bool init();
		void shutdown();
	}

	namespace fonts {
		extern IDWriteTextFormat* watermark_font;
		extern IDWriteTextFormat* tahoma_font;
		extern IDWriteTextFormat* header_tahoma_font;
		extern IDWriteTextFormat* futura_font;
		bool init();
		void shutdown();
	}

	namespace scene {
		void start(); 
		void begin();
		void end();
		void clear();
		void shutdown();

		void text(Vector2 pos, const wchar_t* text, ID2D1SolidColorBrush* color, IDWriteTextFormat* font);
		void textDropShadowStr(Vector2 pos, ID2D1SolidColorBrush* color, IDWriteTextFormat* font, const char* text, ...);
		void textDropShadow(Vector2 pos, const wchar_t* text, ID2D1SolidColorBrush* color, IDWriteTextFormat* font);
		void circle(Vector2 pos, ID2D1SolidColorBrush* color, float radius);
		void box(int x, int y, int width, int height, float thickness, ID2D1SolidColorBrush* color);
		void boxStyle1(int x, int y, int width, int height, float thickness, ID2D1SolidColorBrush* color);
		void boxTransparentFill(int x, int y, int width, int height, float thickness, ID2D1SolidColorBrush* color);
		void filledbox(int x, int y, int width, int height, ID2D1SolidColorBrush* color);
		void line(float x1, float y1, float x2, float y2, float thickness, ID2D1SolidColorBrush* color);

		namespace menu {

			extern Vector2 position;

			namespace mouse {
				Vector2 GetPos();
				bool GetClick();
				bool GetMouseDown();
			}

			void Checkbox(int x, int y, int width, int height, bool& result, ID2D1SolidColorBrush* backColor, ID2D1SolidColorBrush* checkColor, ID2D1SolidColorBrush* hoverColor = colors::light_gray_color);
			void Plane(int x, int y, int width, int height, ID2D1SolidColorBrush* color);
			void DraggablePlane(int x, int y, int width, int height, ID2D1SolidColorBrush* color);
			void Text(Vector2 pos, const wchar_t* text, ID2D1SolidColorBrush* color, IDWriteTextFormat* font);
			void Image(int x, int y, int width, int height, ID2D1Bitmap* bitmap);
			void Space(Vector2 pos, int length, ID2D1SolidColorBrush* color);
			void Rect(int x, int y, int width, int height, float thickness, ID2D1SolidColorBrush* color);
			void ValuePicker(int x, int y, int width, int height, int& result, ID2D1SolidColorBrush* Color, ID2D1SolidColorBrush* textColor, ID2D1SolidColorBrush* secondaryColor);

			class Button
			{
			public:

				bool isClicked;

				void Draw(int x, int y, int width, int height, ID2D1SolidColorBrush* backColor);
			};
		}
	}

	bool init();
	void shutdown();
}