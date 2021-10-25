#include "stdafx.h"
#include "FontRender.h"

namespace Game
{
	void FontRender::Render(RenderContext& rc)
	{
		m_font.Begin(rc);
		m_font.Draw(m_text.c_str(), m_position, m_color, m_rotation,m_scale, m_pivot);
		m_font.End(rc);
	}
}