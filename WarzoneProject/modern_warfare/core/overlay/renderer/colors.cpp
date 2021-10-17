#include "renderer.h"

namespace renderer {
	namespace colors {
		ID2D1SolidColorBrush* white_color = nullptr;
		ID2D1SolidColorBrush* red_color = nullptr;
		ID2D1SolidColorBrush* darkred_color = nullptr;
		ID2D1SolidColorBrush* green_color = nullptr;
		ID2D1SolidColorBrush* blue_color = nullptr;
		ID2D1SolidColorBrush* black_color = nullptr;
		ID2D1SolidColorBrush* cyan_color = nullptr;
		ID2D1SolidColorBrush* orange_color = nullptr;
		ID2D1SolidColorBrush* gray_color = nullptr;
		ID2D1SolidColorBrush* dark_gray_color = nullptr;
		ID2D1SolidColorBrush* dark_gray_color2 = nullptr;
		ID2D1SolidColorBrush* light_gray_color = nullptr;
		ID2D1SolidColorBrush* light_gray_color2 = nullptr;
		ID2D1SolidColorBrush* light_gray_color3 = nullptr;
		ID2D1SolidColorBrush* light_gray_color4 = nullptr;

		bool init( ) {
			internal::render_target->CreateSolidColorBrush( D2D1::ColorF( D2D1::ColorF::White ), &white_color );
			internal::render_target->CreateSolidColorBrush( D2D1::ColorF( D2D1::ColorF::Red ), &red_color );
			internal::render_target->CreateSolidColorBrush( D2D1::ColorF( D2D1::ColorF::OrangeRed ), &darkred_color);

			internal::render_target->CreateSolidColorBrush( D2D1::ColorF( D2D1::ColorF(1, 0.415, 0, 1) ), &orange_color);
			internal::render_target->CreateSolidColorBrush( D2D1::ColorF( D2D1::ColorF(0.149, 0.149, 0.149, 1) ), &dark_gray_color);
			internal::render_target->CreateSolidColorBrush( D2D1::ColorF( D2D1::ColorF(0.2, 0.2, 0.2, 1) ), &dark_gray_color2);
			internal::render_target->CreateSolidColorBrush( D2D1::ColorF( D2D1::ColorF(0.250, 0.250, 0.250, 1) ), &light_gray_color);
			internal::render_target->CreateSolidColorBrush( D2D1::ColorF( D2D1::ColorF(0.350, 0.350, 0.350, 1) ), &light_gray_color2);
			internal::render_target->CreateSolidColorBrush( D2D1::ColorF( D2D1::ColorF(0.450, 0.450, 0.450, 1) ), &light_gray_color3);
			internal::render_target->CreateSolidColorBrush( D2D1::ColorF( D2D1::ColorF(0.850, 0.850, 0.850, 1) ), &light_gray_color4);
			internal::render_target->CreateSolidColorBrush( D2D1::ColorF( D2D1::ColorF::Green ), &green_color );
			internal::render_target->CreateSolidColorBrush( D2D1::ColorF( D2D1::ColorF::Blue ), &blue_color );
			internal::render_target->CreateSolidColorBrush( D2D1::ColorF( D2D1::ColorF::Black ), &black_color );
			internal::render_target->CreateSolidColorBrush( D2D1::ColorF( D2D1::ColorF::Cyan ), &cyan_color);
			internal::render_target->CreateSolidColorBrush( D2D1::ColorF( D2D1::ColorF::Gray ), &gray_color);
			return true;
		}

		void shutdown( ) {
			white_color->Release( );
			red_color->Release( );
			darkred_color->Release( );
			green_color->Release( );
			blue_color->Release( );
			black_color->Release( );
			cyan_color->Release();
			orange_color->Release();
			dark_gray_color->Release();
			light_gray_color->Release();
			light_gray_color2->Release();
			light_gray_color3->Release();
			light_gray_color4->Release();
		}
	}
}
