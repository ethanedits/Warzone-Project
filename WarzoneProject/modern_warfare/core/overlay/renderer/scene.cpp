#include "renderer.h"
#include "../../game/sdk.h"
#include <iostream>


namespace renderer {
	namespace scene {
		void start() {
			begin();
			clear();
		}

		void begin() {
			internal::render_target->BeginDraw();
		}
		
		void end() {
			internal::render_target->EndDraw();

			// code below is really just for convience, without the if statement you would have to
			// manually close each window as when clicking onto the game the focus doesnt close the 
			// other window. Comment it out and u will see what I mean. the statement fixes it
			// by brigning overlay to topmost (removes flag imdeallty) then putting game winodw
			// right under it 

			static bool toggle = true;
			if (GetForegroundWindow() == sdk::hwnd) {
				if (toggle) {
					SetWindowPos(hijack::hijacked_hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
					SetWindowPos(hijack::hijacked_hwnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
					ShowWindow(hijack::hijacked_hwnd, SW_SHOW);
					toggle = false;
				}
			}
			else {
				ShowWindow(hijack::hijacked_hwnd, SW_HIDE);
				toggle = true;
			}

			// sets game window under overlay
			SetWindowPos(sdk::hwnd, hijack::hijacked_hwnd, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		}

		void clear() {
			internal::render_target->Clear();
		}

		void text(Vector2 pos, const wchar_t* text, ID2D1SolidColorBrush* color, IDWriteTextFormat* font) {
			internal::render_target->DrawTextA( text, lstrlenW( text ), font, pos.rect(), color, D2D1_DRAW_TEXT_OPTIONS_NONE, DWRITE_MEASURING_MODE_NATURAL );
		}

		void textDropShadow(Vector2 pos, const wchar_t* text, ID2D1SolidColorBrush* color, IDWriteTextFormat* font)
		{
			Vector2 dropPos = pos; dropPos.x = pos.x - 2; //Offset Text
			internal::render_target->DrawTextA(text, lstrlenW(text), font, dropPos.rect(), renderer::colors::black_color, D2D1_DRAW_TEXT_OPTIONS_NONE, DWRITE_MEASURING_MODE_NATURAL); //Shadow
			internal::render_target->DrawTextA(text, lstrlenW(text), font, pos.rect(), color, D2D1_DRAW_TEXT_OPTIONS_NONE, DWRITE_MEASURING_MODE_NATURAL); //Actual Text
		}

		void textDropShadowStr(Vector2 pos, ID2D1SolidColorBrush* color, IDWriteTextFormat* font, const char* text, ...)
		{
			char buf[4096];
			int len = 0;
			wchar_t b[256];

			va_list arg_list;
			va_start(arg_list, text);
			vsnprintf(buf, sizeof(buf), text, arg_list);
			va_end(arg_list);

			len = strlen(buf);
			mbstowcs(b, buf, len);

			Vector2 dropPos = pos; dropPos.x = pos.x - 2; //Offset Text
			internal::render_target->DrawTextA(b, len, font, dropPos.rect(), renderer::colors::black_color, D2D1_DRAW_TEXT_OPTIONS_NONE, DWRITE_MEASURING_MODE_NATURAL); //Shadow
			internal::render_target->DrawTextA(b, len, font, pos.rect(), color, D2D1_DRAW_TEXT_OPTIONS_NONE, DWRITE_MEASURING_MODE_NATURAL); //Actual Text
		}

		void circle(Vector2 pos, ID2D1SolidColorBrush* color, float radius)
		{
			internal::render_target->DrawEllipse(D2D1_ELLIPSE(D2D1_POINT_2F(pos.x, pos.y), radius, radius), color, 1.f);
		}

		void line(float x1, float y1, float x2, float y2, float thickness, ID2D1SolidColorBrush* color)
		{
			internal::render_target->DrawLine(D2D1_POINT_2F(x1, y1), D2D1_POINT_2F(x2, y2), color, thickness);
		}

		void box(int x, int y, int width, int height, float thickness, ID2D1SolidColorBrush* color)
		{
			internal::render_target->DrawRectangle(D2D1::RectF(x, y, width, height), color, thickness);
		}
		
		void boxStyle1(int x, int y, int width, int height, float thickness, ID2D1SolidColorBrush* color)
		{
			internal::render_target->DrawRectangle(D2D1::RectF(x, y, width, height), color, thickness);

			internal::render_target->DrawRectangle(D2D1::RectF(x - thickness, (y-thickness) + thickness*2, width + thickness, height-thickness + (thickness/12)), colors::black_color, thickness); //Outside Box
			internal::render_target->DrawRectangle(D2D1::RectF(x + thickness, (y + thickness) - thickness * 2, width + (thickness / 8) - thickness, height + thickness - (thickness / 12)), colors::black_color, thickness); //Inside Box
		}

		void boxTransparentFill(int x, int y, int width, int height, float thickness, ID2D1SolidColorBrush* color)
		{
			if (color == colors::red_color)
				color->SetColor(D2D1::ColorF(1, 0, 0, 0.2f)); //Red
			if (color == colors::green_color)
				color->SetColor(D2D1::ColorF(0, 1, 0, 0.2f)); //Red
			internal::render_target->FillRectangle(D2D1::RectF(x, y, width, height), color);
	

			if (color == colors::red_color)
				color->SetColor(D2D1::ColorF(1, 0, 0, 1.f)); //Red
			if (color == colors::green_color)
				color->SetColor(D2D1::ColorF(0, 1, 0, 1.f)); //Red
		}

		void filledbox(int x, int y, int width, int height, ID2D1SolidColorBrush* color)
		{
			internal::render_target->FillRectangle(D2D1::RectF(x, y, x + width, y + height), color);
		}

		namespace menu {
			//Notes for detecting if mouse is within checkbox
			//self.x + self.width > mouseX > self.x && self.y + self.height > mouseY > self.y //Use my oGUI src to help with math

			Vector2 position = Vector2(500,300);

			namespace mouse {
				Vector2 GetPos()
				{
					POINT p;
					GetCursorPos(&p);
					return Vector2(p.x, p.y);
				}

				bool GetClick()
				{
					//using namespace std::this_thread;
					//using namespace std::chrono_literals;
					Sleep(50);

					if ((GetKeyState(VK_LBUTTON) & 0x0202) != 0)
						return true;
					else
						return false;
				}

				bool GetMouseDown()
				{
					if ((GetKeyState(VK_LBUTTON)))
						return true;
					else
						return false;
				}
			}

			void Button::Draw(int x, int y, int width, int height, ID2D1SolidColorBrush* backColor) {
				//Background Box
				filledbox(x, y, width, height, backColor);

				Vector2 mousePos = mouse::GetPos();

				if (sdk::inRange(x, x + width, mousePos.x) && sdk::inRange(y, y + height, mousePos.y)) {
					if (mouse::GetClick()) {
						isClicked = true;
					}
					else
					{
						isClicked = false;
					}
				}
			}

			void ValuePicker(int x, int y, int width, int height, int& result, ID2D1SolidColorBrush* Color, ID2D1SolidColorBrush* textColor, ID2D1SolidColorBrush* secondaryColor)
			{
				int xOffset = 2;
				Vector2 mousePos = mouse::GetPos();
				//Text Display
				filledbox(x + xOffset, y, width, height, Color);

				wchar_t txt[6];
				swprintf(txt, sizeof(txt), L"%d", result);

				if (sdk::inRange(0, 9, result))
					text(Vector2((x + xOffset) + (width/2 - 4), y + (height/5)), txt, textColor, renderer::fonts::header_tahoma_font);
				else if (sdk::inRange(10, 99, result))
					text(Vector2((x + xOffset) + (width / 2 - 8), y + (height / 5)), txt, textColor, renderer::fonts::header_tahoma_font);
				else
					text(Vector2((x + xOffset) + (width / 2 - 12), y + (height / 5)), txt, textColor, renderer::fonts::header_tahoma_font);
				

				//Left Val Changer
				filledbox(((x + xOffset) - width/2) + 6, y, width/3, height, Color); filledbox(((x + xOffset) - width / 2) + 7, y + 15, 14, 2, secondaryColor); //Minus Symbol
				if (sdk::inRange(((x + xOffset) - width / 2) + 6, ((x + xOffset) - width / 2) + 6 + width, mousePos.x) && sdk::inRange(y, y + height, mousePos.y)) {

					filledbox(((x + xOffset) - width / 2) + 6, y, width / 3, height, renderer::colors::light_gray_color3); filledbox(((x + xOffset) - width / 2) + 7, y + 15, 14, 2, secondaryColor); //Minus Symbol
					if (mouse::GetClick()) {
						result -= 1;
					}
				}

				//Right Val Changer
				filledbox(((x - xOffset) + width / 2) + 32, y, width / 3, height, Color); filledbox(((x - xOffset) + width / 2) + 33, y + 15, 14, 2, secondaryColor); filledbox(((x - xOffset) + width / 2) + 39, y + 9, 2, 14, secondaryColor); //Addition Symbol
				if (sdk::inRange(((x - xOffset) + width / 2) + 32, ((x - xOffset) + width / 2) + 32 + width, mousePos.x) && sdk::inRange(y, y + height, mousePos.y)) {

					filledbox(((x - xOffset) + width / 2) + 32, y, width / 3, height, renderer::colors::light_gray_color3); filledbox(((x - xOffset) + width / 2) + 33, y + 15, 14, 2, secondaryColor); filledbox(((x - xOffset) + width / 2) + 39, y + 9, 2, 14, secondaryColor); //Addition Symbol
					if (mouse::GetClick()) {
						result += 1;
					}
				}
			}

			void Checkbox(int x, int y, int width, int height, bool& result, ID2D1SolidColorBrush* backColor, ID2D1SolidColorBrush* checkColor, ID2D1SolidColorBrush* hoverColor)
			{
				//Background Box
				filledbox(x, y, width, height, backColor);

				Vector2 mousePos = mouse::GetPos();

				if (sdk::inRange(x, x + width, mousePos.x) && sdk::inRange(y, y + height, mousePos.y)) {
					filledbox(x, y, width, height, hoverColor);
					if (mouse::GetClick()) {
						result = !result;
					}
				}

				//Check Box
				if (result)
					filledbox(x + width / 7, y + height / 7, width - width / 4.1, height - height / 4.1, checkColor);
			}

			void Plane(int x, int y, int width, int height, ID2D1SolidColorBrush* color)
			{
				filledbox(x, y, width, height, color);
			}

			void DraggablePlane(int x, int y, int width, int height, ID2D1SolidColorBrush* color)
			{
				filledbox(x, y, width, height, color);

				Vector2 mousePos = mouse::GetPos();

				bool canBeDragged;

				//if (sdk::inRange(x, x + width, mousePos.x) && sdk::inRange(y, y + height, mousePos.y)) {
				//	while (mouse::GetMouseDown())
				//	{
				//		//position = Vector2(mouse::GetPos().x + (mouse::GetPos().x - position.x), mouse::GetPos().y);
				//		position = mouse::GetPos();
				//	}
				//}

				while (mouse::GetMouseDown())
				{
					position = Vector2(mouse::GetPos().x + (mouse::GetPos().x - position.x), mouse::GetPos().y);
				}
			}

			void Rect(int x, int y, int width, int height, float thickness, ID2D1SolidColorBrush* color)
			{
				internal::render_target->DrawRectangle(D2D1::RectF(x, y, x + width, y + height), color, thickness);
			}

			//x, y, x + width, y + height

			void Text(Vector2 pos, const wchar_t* text, ID2D1SolidColorBrush* color, IDWriteTextFormat* font)
			{
				Vector2 dropPos = pos; dropPos.x = pos.x - 2; //Offset Text
				internal::render_target->DrawTextA(text, lstrlenW(text), font, dropPos.rect(), renderer::colors::black_color, D2D1_DRAW_TEXT_OPTIONS_NONE, DWRITE_MEASURING_MODE_NATURAL); //Shadow
				internal::render_target->DrawTextA(text, lstrlenW(text), font, pos.rect(), color, D2D1_DRAW_TEXT_OPTIONS_NONE, DWRITE_MEASURING_MODE_NATURAL); //Actual Text
			}

			void Space(Vector2 pos, int length, ID2D1SolidColorBrush* color)
			{
				line(pos.x, pos.y, pos.x + length, pos.y, 2.f, color);
			}

			void Image(int x, int y, int width, int height, ID2D1Bitmap* bitmap)
			{
				//Need to load/create bitmap or something
				D2D1_SIZE_F size = bitmap->GetSize();
				internal::render_target->DrawBitmap(bitmap, D2D1::RectF(x, y, x + width, y + width), 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, D2D1::RectF(0, 0, size.width, size.height));
			}
		}

		void shutdown() {
			begin();
			clear();
			end();
		}
	}
}