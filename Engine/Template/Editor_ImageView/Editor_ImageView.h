#pragma once
#include "../../Component/_Exclude.h"

class EditorDetailWindow_ImageView : public EditorDetailWindow
{
public:
	enum class MODE {
		CENTER_SQUARE, FULL_BOX, CENTER_SPECIFIC
	};
	struct ImageElum
	{
		std::string display_name;
		SafePtr<TEXTURE::Texture> texture;
		ImageElum() {};
		ImageElum(std::string display_name, SafePtr<TEXTURE::Texture> texture) : display_name(display_name), texture(texture) {}
	};

	MODE mode = MODE::FULL_BOX;
	std::string window_name;

	std::vector<ImageElum> allow_textures;
	SafePtr<TEXTURE::Texture> allow_texture;

	UINT width = 0, height = 0;

	virtual std::type_index get_class_type();
	// draw_detail_view에서 윈도우가 그려진다.
	virtual void draw_detail_view() override;
	// 여기서 component가 호출하여 디스플레이 된다..
	virtual void detail_window_detail_view();
};

// 개선 해야함.
class Editor_ImageView : public Component
{
public:
	int draw_index = 0;
	std::vector<SafePtr<TEXTURE::Texture>> allow_textures;

	Editor_ImageView(std::vector<SafePtr<TEXTURE::Texture>> allow_textures);

	virtual ~Editor_ImageView();

	virtual void draw_detail_view() override;
};

namespace EDITOR
{
	class Defered_renderer_to_image_view
	{
	public:
		Defered_renderer_to_image_view* get_instance();
	};
	extern void create_image_view_from_deferred_renderer(
		std::unique_ptr<EditorDetailWindow_ImageView>& edwiv,
		SafePtr<RenderToDeferred> rtd
	);
}