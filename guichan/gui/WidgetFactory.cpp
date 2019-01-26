//
// Created by tatiana on 27.09.18.
//

#include "WidgetFactory.h"
#include "ZButton.h"
#include "ZCheckBox.h"
#include "ZIcon.h"
#include "ZTextBox.h"
#include "ZLabel.h"
#include "ZContainer.h"
#include "ZScrollArea.h"
#include "ZListBox.h"
#include "ZDropDown.h"
#include "ZSlider.h"
#include "ZProgressBar.h"

namespace gui
{
    void DeleteZButton(ZButton* obj)
    {
        obj->shutdown();
        std::cout << "Deleting ZButton\n";
    }

    std::shared_ptr<ZButton> WidgetFactory::create_zbutton()
    {
        auto ptr=std::shared_ptr<ZButton>(new ZButton(),DeleteZButton);
        ptr->init();
        return ptr;
    }

    std::shared_ptr<ZButton> WidgetFactory::create_zbutton(const std::string& caption)
    {
        auto ptr=std::shared_ptr<ZButton>(new ZButton(),DeleteZButton);
        ptr->init(caption);
        return ptr;
    }

    std::shared_ptr<ZCheckBox> WidgetFactory::create_zcheckbox()
    {
        auto ptr=std::make_shared<ZCheckBox>();
        ptr->init();
        return ptr;
    }

    std::shared_ptr<ZCheckBox> WidgetFactory::create_zcheckbox(const std::string &caption, bool selected)
    {
        auto ptr=std::make_shared<ZCheckBox>();
        ptr->init(caption,selected);
        return ptr;
    }

    std::shared_ptr<ZIcon> WidgetFactory::create_zicon()
    {
        auto ptr=std::make_shared<ZIcon>();
        ptr->init();
        return ptr;
    }

    std::shared_ptr<ZIcon> WidgetFactory::create_zicon(const std::string& filename)
    {
        auto ptr=std::make_shared<ZIcon>();
        ptr->init(filename);
        return ptr;
    }

    std::shared_ptr<ZIcon> WidgetFactory::create_zicon(std::shared_ptr<const ZImage> image)
    {
        auto ptr=std::make_shared<ZIcon>();
        ptr->init(image);
        return ptr;
    }

    std::shared_ptr<ZTextBox> WidgetFactory::create_ztextbox()
    {
        auto ptr=std::make_shared<ZTextBox>();
        ptr->init();
        return ptr;
    }

    std::shared_ptr<ZTextBox> WidgetFactory::create_ztextbox(const std::string& text)
    {
        auto ptr=std::make_shared<ZTextBox>();
        ptr->init(text);
        return ptr;
    }

    std::shared_ptr<ZLabel> WidgetFactory::create_zlabel()
    {
        auto ptr=std::make_shared<ZLabel>();
        ptr->init();
        return ptr;
    }

    std::shared_ptr<ZLabel> WidgetFactory::create_zlabel(const std::string& caption)
    {
        auto ptr=std::make_shared<ZLabel>();
        ptr->init(caption);
        return ptr;
    }

    std::shared_ptr<ZScrollArea> WidgetFactory::create_zscrollarea()
    {
        auto ptr=std::make_shared<ZScrollArea>();
        ptr->init();
        return ptr;
    }

    std::shared_ptr<ZScrollArea> WidgetFactory::create_zscrollarea(std::shared_ptr<ZWidget> content)
    {
        auto ptr=std::make_shared<ZScrollArea>();
        ptr->init(content);
        return ptr;
    }

    std::shared_ptr<ZScrollArea> WidgetFactory::create_zscrollarea(std::shared_ptr<ZWidget> content,
                                                           ZScrollArea::ScrollPolicy hPolicy, ZScrollArea::ScrollPolicy vPolicy)
    {
        auto ptr=std::make_shared<ZScrollArea>();
        ptr->init(content,hPolicy,vPolicy);
        return ptr;
    }

    std::shared_ptr<ZListBox> WidgetFactory::create_zlistbox()
    {
        auto ptr=std::make_shared<ZListBox>();
        ptr->init();
        return ptr;
    }

    std::shared_ptr<ZListBox> WidgetFactory::create_zlistbox(std::shared_ptr<ListModel> listModel)
    {
        auto ptr=std::make_shared<ZListBox>();
        ptr->init(listModel);
        return ptr;
    }

    std::shared_ptr<ZDropDown> WidgetFactory::create_zdropdown(std::shared_ptr<ListModel> listModel,
                                                std::shared_ptr<ZScrollArea> scrollArea,std::shared_ptr<ZListBox> listBox)
    {
        auto ptr=std::make_shared<ZDropDown>();
        ptr->init(listModel,scrollArea,listBox);
        return ptr;
    }

    std::shared_ptr<ZSlider> WidgetFactory::create_zslider(double scaleEnd)
    {
        auto ptr=std::make_shared<ZSlider>();
        ptr->init(scaleEnd);
        return ptr;
    }

    std::shared_ptr<ZSlider> WidgetFactory::create_zslider(double scaleStart, double scaleEnd)
    {
        auto ptr=std::make_shared<ZSlider>();
        ptr->init(scaleStart,scaleEnd);
        return ptr;
    }

    void DeleteZProgressBar(ZProgressBar* obj)
    {
        obj->shutdown();
        std::cout << "Deleting ZProgressBar\n";
    }

    std::shared_ptr<ZProgressBar> WidgetFactory::create_zprogressbar()
    {
        auto ptr=std::shared_ptr<ZProgressBar>(new ZProgressBar(),DeleteZProgressBar);
        ptr->init();
        return ptr;
    }

    std::shared_ptr<ZContainer> WidgetFactory::create_zcontainer()
    {
        auto ptr=std::make_shared<ZContainer>();
        ptr->init();
        return ptr;
    }
}