# pragma once
# include <Siv3D.hpp>
# include "Axis.hpp"
# include "AxisGroup.hpp"

namespace asc
{
	using namespace s3d;

	template<class NameType>
	class Input
	{
	private:

		std::unordered_map<NameType, KeyGroup> m_keyGroups;
		std::unordered_map<NameType, AxisGroup> m_axisGroups;

	public:

		/// <summary>
		/// キーが登録済みかを返します。
		/// </summary>
		/// <param name="name">
		/// 登録したキーの名前
		/// </param>
		/// <returns>
		/// true キーが登録されている / false キーが登録されていない
		/// </returns>
		[[nodiscard]] bool isKeyRegistered(const NameType& name) const
		{
			return m_keyGroups.find(name) != m_keyGroups.end();
		}

		/// <summary>
		/// 軸が登録済みかを返します。
		/// </summary>
		/// <param name="name">
		/// 登録した軸の名前
		/// </param>
		/// <returns>
		/// true 軸が登録されている / false 軸が登録されていない
		/// </returns>
		[[nodiscard]] bool isAxisRegistered(const NameType& name) const
		{
			return m_axisGroups.find(name) != m_axisGroups.end();
		}

		/// <summary>
		/// キーを登録します。
		/// </summary>
		/// <param name="name">
		/// 登録するキーの名前
		/// </param>
		/// <param name="key">
		/// 登録するキー
		/// </param>
		/// <returns>
		/// なし
		/// </returns>
		void regsiterKey(const NameType& name, const Key& key)
		{
			registerKey(name, KeyGroup(key));
		}

		/// <summary>
		/// キーを登録します。
		/// </summary>
		/// <param name="name">
		/// 登録するキーの名前
		/// </param>
		/// <param name="group">
		/// 登録するキー
		/// </param>
		/// <returns>
		/// なし
		/// </returns>
		void regsiterKey(const NameType& name, const KeyGroup& group)
		{
			m_keyGroups[name] = group;
		}

		/// <summary>
		/// 軸を登録します。
		/// </summary>
		/// <param name="name">
		/// 登録する軸の名前
		/// </param>
		/// <param name="axis">
		/// 登録する軸
		/// </param>
		/// <returns>
		/// なし
		/// </returns>
		void regsiterAxis(const NameType& name, const Axis& axis)
		{
			regsiterAxis(name, AxisGroup(axis));
		}

		/// <summary>
		/// 軸を登録します。
		/// </summary>
		/// <param name="name">
		/// 登録する軸の名前
		/// </param>
		/// <param name="axis">
		/// 登録する軸
		/// </param>
		/// <returns>
		/// なし
		/// </returns>
		void regsiterAxis(const NameType& name, const AxisGroup& group)
		{
			m_axisGroups[name] = group;
		}

		/// <summary>
		/// キーを削除します。
		/// </summary>
		/// <param name="name">
		/// 削除するキーの名前
		/// <returns>
		/// なし
		/// </returns>
		void unregsiterKey(const NameType& name)
		{
			m_keyGroups.erase(name);
		}

		/// <summary>
		/// 軸を削除します。
		/// </summary>
		/// <param name="name">
		/// 削除する軸の名前
		/// <returns>
		/// なし
		/// </returns>
		void unregsiterAxis(const NameType& name)
		{
			m_axisGroups.erase(name);
		}

		/// <summary>
		/// 指定したキーを返します。
		/// </summary>
		/// <param name="name">
		/// 登録したキーの名前
		/// </param>
		/// <returns>
		/// 登録したキー
		/// </returns>
		[[nodiscard]] const KeyGroup& key(const NameType& name) const
		{
			return m_keyGroups.at(name);
		}

		/// <summary>
		/// 指定した軸を返します。
		/// </summary>
		/// <param name="name">
		/// 登録した軸の名前
		/// </param>
		/// <returns>
		/// 登録した軸
		/// </returns>
		[[nodiscard]] const AxisGroup& axis(const NameType& name) const
		{
			return m_axisGroups.at(name);
		}

		/// <summary>
		/// 2つの軸から Vec2 を作成します。
		/// </summary>
		/// <param name="xName">
		/// X軸にする登録したボタンの名前
		/// </param>
		/// <param name="yName">
		/// Y軸にする登録したボタンの名前
		/// </param>
		/// <param name="threshold">
		/// 軸の閾値、この閾値以下の入力は無視されます。
		/// </param>
		/// <returns>
		/// { xName, yName } の Vec2
		/// </returns>
		[[nodiscard]] Vec2 vec2(const NameType& xName, const NameType& yName, double threshold = 0.12) const
		{
			assert(isAxisRegistered(xName));
			assert(isAxisRegistered(yName));

			return { axis(xName).get(threshold), axis(yName).get(threshold) };
		}

		/// <summary>
		/// 2つの軸から Vec2 を作成します。
		/// </summary>
		/// <param name="xName">
		/// X軸にする登録したボタンの名前
		/// </param>
		/// <param name="yName">
		/// Y軸にする登録したボタンの名前
		/// </param>
		/// <param name="threshold">
		/// 軸の閾値、この閾値以下の入力は無視されます。
		/// </param>
		/// <returns>
		/// { xName, yName } の正規化された Vec2
		/// </returns>
		/// <remarks>
		/// 零ベクトルの場合は零ベクトルを返します。
		/// </remarks>
		[[nodiscard]] Vec2 vec2Normalized(const NameType& xName, const NameType& yName, double threshold = 0.12) const
		{
			const auto rawValue = vec2(xName, yName, threshold);

			if (rawValue.isZero())
			{
				return Vec2::Zero();
			}

			return rawValue.normalized();
		}

		/// <summary>
		/// 2つの軸から方向を [0, 3] で返します。
		/// </summary>
		/// <param name="xName">
		/// X軸にする登録したボタンの名前
		/// </param>
		/// <param name="yName">
		/// Y軸にする登録したボタンの名前
		/// </param>
		/// <param name="threshold">
		/// 軸の閾値、この閾値以下の入力は無視されます。
		/// </param>
		/// <returns>
		/// [0, 3] で表現された方向
		/// </returns>
		[[nodiscard]] Optional<uint32> as4Direction(const NameType& xName, const NameType& yName, double threshold = 0.2)
		{
			const auto rawValue = vec2(xName, yName, threshold);
			const Circular circular = Vec2{ rawValue.x, -rawValue.y };

			if (circular.r < threshold)
			{
				return none;
			}

			const double deg = Math::ToDegrees(circular.theta);

			if (deg <= -135.0)
			{
				return 2u;
			}
			else if (deg <= -45.0)
			{
				return 3u;
			}
			else if (deg <= 45.0)
			{
				return 0u;
			}
			else if (deg <= 135.0)
			{
				return 1u;
			}
			else
			{
				return 2u;
			}
		}

		/// <summary>
		/// 2つの軸から方向を [0, 7] で返します。
		/// </summary>
		/// <param name="xName">
		/// X軸にする登録したボタンの名前
		/// </param>
		/// <param name="yName">
		/// Y軸にする登録したボタンの名前
		/// </param>
		/// <param name="threshold">
		/// 軸の閾値、この閾値以下の入力は無視されます。
		/// </param>
		/// <returns>
		/// [0, 7] で表現された方向
		/// </returns>
		[[nodiscard]] Optional<uint32> as8Direction(const NameType& xName, const NameType& yName, double threshold = 0.2)
		{
			const auto rawValue = vec2(xName, yName, vec2::Zero(), threshold);
			const Circular circular = Vec2{ rawValue.x, -rawValue.y };

			if (circular.r < threshold)
			{
				return none;
			}

			const double deg = Math::ToDegrees(circular.theta);

			if (deg <= -157.5)
			{
				return 4u;
			}
			else if (deg <= -112.5)
			{
				return 5u;
			}
			else if (deg <= -67.5)
			{
				return 6u;
			}
			else if (deg <= -22.5)
			{
				return 7u;
			}
			else if (deg <= 22.5)
			{
				return 0u;
			}
			else if (deg <= 67.5)
			{
				return 1u;
			}
			else if (deg <= 112.5)
			{
				return 2u;
			}
			else if (deg <= 157.5)
			{
				return 3u;
			}
			else
			{
				return 4u;
			}
		}
	};
}
