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
#include <util/encoding.h>

#include "HTMLRenderer.h"
#include "util/namespace.h"
#include "util/misc.h"

namespace pdf2htmlEX {

    using std::ofstream;
    using std::cerr;


    void HTMLRenderer::process_form(ofstream &out) {
        FormPageWidgets *widgets = cur_catalog->getPage(pageNum)->getFormWidgets();
        int num = widgets->getNumWidgets();

        for (int i = 0; i < num; i++) {
            FormWidget *w = widgets->getWidget(i);

            double x1, y1, x2, y2;

            w->getRect(&x1, &y1, &x2, &y2);
            x1 = x1 * param.zoom;
            x2 = x2 * param.zoom;
            y1 = y1 * param.zoom;
            y2 = y2 * param.zoom;

            double width = x2 - x1;
            double height = y2 - y1;

            if (w->getType() == formText) {
                FormWidgetText *textWidget = static_cast<FormWidgetText *>(w);
                double font_size = height / 2;

                out << "<input class=\"" << CSS::INPUT_TEXT_CN
                    << "\" name=\"" << w->getFullyQualifiedName()->getCString()
                    << "\" type=\"text\" value=\"";

                writeUnicodes(out, textWidget->getContent());

                out << "\""
                    << "style=\"position: absolute; background-color: #fff; left: " << x1
                    << "px; bottom: " << y1 << "px;"
                    << " width: " << width << "px; height: " << std::to_string(height)
                    << "px; line-height: " << std::to_string(height) << "px; font-size: "
                    << font_size << "px;\" />" << endl;

            } else if (w->getType() == formButton) {
                FormWidgetButton *button = static_cast<FormWidgetButton *>(w);
                const char *checked = button->getState() ? "checked=\"checked\"" : "";

                out << "<input name=\"" << w->getFullyQualifiedName()->getCString()
                    << "\" type=\"checkbox\""
                    << checked
                    << "style=\"position: absolute; border: 1px solid #000; background-color: white; left: " << x1
                    << "px; bottom: " << y1 << "px;"
                    << " width: " << width << "px; height: "
                    << std::to_string(height) << "px;\" >" << endl;
            } else {
                cerr << "Unsupported form field detected" << endl;
            }
        }
    }

}
