#include "UIWidget.h"
#include "include/core/SkCanvas.h"
#include "include/core/SkFont.h"
#include "include/effects/SkGradientShader.h"
#include "include/core/SkGraphics.h"
#include "include/core/SkImage.h"

//�ָ���
typedef std::function<void(UIWidget *p,SkPoint point)> SplitDragCallBack;
class SplitView :public UIWidget {
public:
	SplitView();
	~SplitView() {};
	void Draw(SkCanvas* canvas) override;
	void OnMouseMove(int x, int y) override;
	bool OnMouseDown(int x, int y) override;
	bool OnMouseUp(int x, int y) override;
	void OnMouseWheel(float delta, uint32_t modifier) override {};

	//������Ļص�
	void SetDragCallBack(SplitDragCallBack f)
	{
		funcc = f;
	}

	void SetMouseDragged(bool bValue) {
		bMouseDragged = bValue;
	}

	bool GetMouseDragged()
	{
		return bMouseDragged;
	}

	enum Direction {
		Vertical,  //��ֱ����
		Horizontal //ˮƽ����
	};
private:
	SplitDragCallBack funcc;
	bool bMouseDragged;
	Direction nDirectionType;
	int mouse_offset;
};