# include <Siv3D.hpp>
# include "asc/Input.hpp"

void Main()
{
	asc::Input<String> input;

	// キーの登録
	input.regsiterKey(
		U"submit",
		XInput(0).buttonA | Gamepad(0).buttons[3] | KeyZ
	);

	input.regsiterKey(
		U"cancel",
		XInput(0).buttonB | Gamepad(0).buttons[4] | KeyX
	);

	// XInput の軸の作成
	asc::Axis xinputAxis = asc::Axis(0, asc::XInputAxisType::LeftThumbX);

	// ゲームパッドの軸の作成
	asc::Axis gamepadAxis = asc::Axis(0, 0);

	// キーで表される軸の作成
	asc::Axis keyAxis = asc::Axis(KeyLeft, KeyRight);

	// 軸グループの作成
	// 左に記述したものから優先されます。
	asc::AxisGroup axisGroup = xinputAxis | gamepadAxis | keyAxis;

	// 軸の登録
	input.regsiterAxis(U"horizontal", axisGroup);

	input.regsiterAxis(
		U"vertical",
		asc::Axis(0, asc::XInputAxisType::LeftThumbY) | asc::Axis(0, 1) | asc::Axis(KeyDown, KeyUp)
	);

	// XInput の DeadZone の設定
	auto xinput = XInput(0);

	if (xinput.isConnected())
	{
		xinput.setLeftThumbDeadZone();
	}

	while (System::Update())
	{
		ClearPrint();

		// キーの取得
		Print << U"submit:{}"_fmt(input.key(U"submit").pressed());
		Print << U"cancel:{}"_fmt(input.key(U"cancel").pressed());

		// 軸の取得
		Print << U"horizontal:{}"_fmt(input.axis(U"horizontal"));
		Print << U"vertical:{}"_fmt(input.axis(U"vertical"));

		// 軸を正規化された Vec2 として取得
		Print << U"(horizontal, vertical):{}"_fmt(input.vec2Normalized(U"horizontal", U"vertical"));

		// 軸を [0, 3] で表される方向として取得
		Print << U"4Direction:{}"_fmt(input.as4Direction(U"horizontal", U"vertical"));
	}
}
