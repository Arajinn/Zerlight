//
// Created by tatiana on 27.09.18.
//

#ifndef GUICHAN_WIDGETFACTORY_H
#define GUICHAN_WIDGETFACTORY_H

#include "guichan/gui/ZScrollArea.h"

#include <memory>

namespace gui {
    class ZButton;
    class ZCheckBox;
    class ZIcon;
    class ZImage;
    class ZTextBox;
    class ZLabel;
    class ZContainer;
    class ZListBox;
    class ListModel;
    class ZDropDown;
    class ZSlider;
    class ZProgressBar;

    class WidgetFactory {
    public:
        static std::shared_ptr<ZButton> create_zbutton();
        static std::shared_ptr<ZButton> create_zbutton(const std::string& caption);

        static std::shared_ptr<ZCheckBox> create_zcheckbox();
        static std::shared_ptr<ZCheckBox> create_zcheckbox(const std::string &caption, bool selected = false);

        static std::shared_ptr<ZIcon> create_zicon();
        static std::shared_ptr<ZIcon> create_zicon(const std::string& filename);
        static std::shared_ptr<ZIcon> create_zicon(std::shared_ptr<const ZImage> image);

        static std::shared_ptr<ZTextBox> create_ztextbox();
        static std::shared_ptr<ZTextBox> create_ztextbox(const std::string& text);

        static std::shared_ptr<ZLabel> create_zlabel();
        static std::shared_ptr<ZLabel> create_zlabel(const std::string& caption);

        static std::shared_ptr<ZScrollArea> create_zscrollarea();
        static std::shared_ptr<ZScrollArea> create_zscrollarea(std::shared_ptr<ZWidget> content);
        static std::shared_ptr<ZScrollArea> create_zscrollarea(std::shared_ptr<ZWidget> content,
                ZScrollArea::ScrollPolicy hPolicy, ZScrollArea::ScrollPolicy vPolicy);

        static std::shared_ptr<ZListBox> create_zlistbox();
        static std::shared_ptr<ZListBox> create_zlistbox(std::shared_ptr<ListModel> listModel);

        static std::shared_ptr<ZDropDown> create_zdropdown(std::shared_ptr<ListModel> listModel = nullptr,
                                                           std::shared_ptr<ZScrollArea> scrollArea = nullptr,
                                                           std::shared_ptr<ZListBox> listBox = nullptr);

        static std::shared_ptr<ZSlider> create_zslider(double scaleEnd = 1.0);
        static std::shared_ptr<ZSlider> create_zslider(double scaleStart, double scaleEnd);

        static std::shared_ptr<ZProgressBar> create_zprogressbar();

        static std::shared_ptr<ZContainer> create_zcontainer();
    };
}

#endif //GUICHAN_WIDGETFACTORY_H
