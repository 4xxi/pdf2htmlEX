/*
 * form.cc
 *
 * Handling Forms
 *
 * by Simon Chenard
 * 2014.07.25
 */

#include <iostream>
#include <sstream>
#include <string>
#include <codecvt>
#include <util/encoding.h>
#include <glib.h>
#include "HTMLRenderer.h"
#include "util/namespace.h"
#include "util/misc.h"

#define TO_STR(V) std::to_string((V))

std::ostream& operator<<(std::ostream& out, GooString* data)
{
    if (data!=nullptr) {
        gsize writeBytes = 0;
        gchar* utf8 = g_convert(data->getCString(), data->getLength(), "UTF-8", "UTF-16BE", NULL, &writeBytes, NULL);
        out.write(utf8, writeBytes);

        // free memory
        free(utf8);
    }

    return out;
}

namespace pdf2htmlEX {

    using std::ofstream;
    using std::cerr;

    void HTMLRenderer::process_form(ofstream& out)
    {
        FormPageWidgets* widgets = cur_catalog->getPage(pageNum)->getFormWidgets();
        int num = widgets->getNumWidgets();

        for (int i = 0; i<num; i++) {
            FormWidget* w = widgets->getWidget(i);

            double x1, y1, x2, y2;

            w->getRect(&x1, &y1, &x2, &y2);
            x1 = x1*param.zoom;
            x2 = x2*param.zoom;
            y1 = y1*param.zoom;
            y2 = y2*param.zoom;

            double width = x2-x1;
            double height = y2-y1;

            if (w->getType()==formText) {
                width -= 2;
                height -= 2;

                FormWidgetText* textWidget = static_cast<FormWidgetText*>(w);
                double font_size = height-2;
                int maxLength = textWidget->getMaxLen()>0 ? textWidget->getMaxLen() : 900000;

                out << "<input class=\"" << CSS::INPUT_TEXT_CN
                    << "\" name=\"" << w->getFullyQualifiedName()
                    << "\" maxlength=\"" << TO_STR(maxLength)
                    << "\" type=\"text\" value=\""
                    << textWidget->getContent() << "\""
                    << "style=\"position: absolute; background-color: #fff; padding: 0px; margin: 1px; left: " << x1
                    << "px; bottom: " << y1 << "px;"
                    << " width: " << width << "px; height: " << TO_STR(height)
                    << "px; line-height: " << TO_STR(height) << "px; font-size: "
                    << font_size << "px;\" />" << endl;

            }
            else if (w->getType()==formButton) {
                width += 1;
                height += 1;
                FormWidgetButton* button = static_cast<FormWidgetButton*>(w);
                const char* checked = button->getState() ? "checked=\"checked\"" : "";

                out << "<input name=\"" << w->getFullyQualifiedName()
                    << "\" type=\"checkbox\""
                    << checked
                    << "data-checked=\"" << button->getOnStr() << "\""
                    << "style=\"position: absolute; border: 1px solid #000; padding: 0px; margin: 0px; background-color: white; left: "
                    << x1
                    << "px; bottom: " << y1 << "px;"
                    << " width: " << width << "px; height: "
                    << TO_STR(height) << "px;\" >" << endl;
            }
            else {
                cerr << "Unsupported form field detected" << endl;
            }
        }
    }

}
