
# include <Siv3D.hpp>
# include "asc/AscInput.hpp"

using namespace asc;

void Main()
{
	asc::Input input;

	// コントローラーを用意
	Gamepad gamepad(0);
	XInput xinput(0);
	xinput.setLeftThumbDeadZone();

	// ボタンの追加
	input.addButton(L"fire", s3d::Input::KeyZ | Gamepad(0).button(2) | XInput(0).buttonA);

	// 軸の追加
	input.addAxis(L"x", Axis(s3d::Input::KeyRight, s3d::Input::KeyLeft) | Axis(gamepad, GamepadAxis::X) | Axis(xinput, XInputAxis::LeftThumbX));
	input.addAxis(L"y", Axis(s3d::Input::KeyUp, s3d::Input::KeyDown) | Axis(gamepad, GamepadAxis::Y) | Axis(xinput, XInputAxis::LeftThumbY));

	while (System::Update())
	{
		ClearPrint();

		// 値の使用
		Println(L"fire = ", input.button(L"fire").clicked);
		Println(L"x = ", input.axis(L"x"));

		// enabledがfalseの間、入力が無効になる
		input.enabled = !input.button(L"fire").pressed;

		Println(L"fire = ", input.button(L"fire").clicked);

		// 零ベクトルを考慮した単位ベクトルを返す
		Println(L"(x,y) = ", input.vec2Normalized(L"x", L"y"));

		// 入力を8方向で返す
		Println(L"(x,y) = ", input.as8Direction(L"x", L"y"));

		input.enabled = true;
	}
}