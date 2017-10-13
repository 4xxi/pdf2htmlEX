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
#include <util/encoding.h>
#include <glib.h>
#include "HTMLRenderer.h"
#include "util/namespace.h"

#define TO_STR(V) std::to_string((V))

const char *UTF_16_BE_BOM = "\xFE\xFF";
const char *UTF_8_BOM = "\xEF\xBB\xBF";

int check_bom(const char *data, size_t size) {

    if (size >= 3 && memcmp(data, UTF_8_BOM, 3) == 0) {
        return 0;
    }
    if (size >= 2 && memcmp(data, UTF_16_BE_BOM, 2) == 0) {
        return 1;
    }

    return -1;
}

std::ostream &operator<<(std::ostream &out, GooString *data) {
    if (data == nullptr) {
        return out;
    }

    if (check_bom(data->getCString(), data->getLength()) == 1) {
        gsize writeBytes = 0;
        gchar *utf8 = g_convert(data->getCString(), data->getLength(), "UTF-8", "UTF-16BE", NULL, &writeBytes, NULL);
        if (writeBytes > 0) {
            out.write(utf8, writeBytes);
        }
        free(utf8);
    } else {
        out.write(data->getCString(), data->getLength());
    }

    return out;
}

class SelectWidget : public FormWidgetChoice {
public:
    SelectWidget(PDFDoc *docA, Object *dict, unsigned int num, const Ref &ref, FormField *p) :
            FormWidgetChoice(docA, dict, num, ref, p) {}

    GooString *getOptionName(int i) {
        return this->parent()->getChoice(i);
    }

    GooString * getOptionVal(int i) {
        return this->parent()->getExportVal(i);
    }
};

namespace pdf2htmlEX {

    using std::ofstream;
    using std::cerr;

    struct InputPosition {
        double x1, x2, y1, y2;

        InputPosition(FormWidget *widget, double zoom) {
            widget->getRect(&x1, &y1, &x2, &y2);
            x1 = x1 * zoom;
            x2 = x2 * zoom;
            y1 = y1 * zoom;
            y2 = y2 * zoom;
        }

        double getWidth() {
            return x2 - x1;
        }

        double getHeight() {
            return y2 - y1;
        }
    };

    struct FormPrint {
        static void print(ofstream &out, InputPosition &pos, FormWidgetText *textWidget) {
            double width = pos.getWidth() - 2;
            double height = pos.getHeight() - 2;
            double fontSize = height - 2;
            int maxLength = textWidget->getMaxLen() > 0 ? textWidget->getMaxLen() : 900000;

            if (textWidget->isMultiline()) {
                fontSize = 12;
                int textareaRows = (int) (width / fontSize);
                textareaRows = textareaRows == 0 ? 1 : textareaRows;

                out << "<textarea class=\"" << CSS::INPUT_TEXT_CN << "\" "
                    << "name=\"" << textWidget->getFullyQualifiedName() << "\" "
                    << "maxlength=\"" << TO_STR(maxLength) << "\" "
                    << "rows=\"" << TO_STR(textareaRows) << "\" "
                    << "type=\"text\" "
                    << "style=\"left: " << pos.x1 << "px; bottom: " << pos.y1 << "px;"
                    << "width: " << width << "px;" << "height: " << TO_STR(height) << "px;"
                    << "font-size: " << fontSize << "px;\""
                    << ">" << textWidget->getContent() << "</textarea>" << endl;
            } else {
                out << "<input class=\"" << CSS::INPUT_TEXT_CN
                    << "\" name=\"" << textWidget->getFullyQualifiedName()
                    << "\" maxlength=\"" << TO_STR(maxLength)
                    << "\" type=\"text\" value=\""
                    << textWidget->getContent() << "\""
                    << "style=\"left: " << pos.x1 << "px; bottom: " << pos.y1 << "px;"
                    << " width: " << width << "px; height: " << TO_STR(height)
                    << "px; line-height: " << TO_STR(height) << "px; font-size: "
                    << fontSize << "px;\" />" << endl;
            }

        }

        static void print(ofstream &out, InputPosition &pos, FormWidgetButton *formWidgetButton) {
            double width = pos.getWidth() + 1;
            double height = pos.getHeight() + 1;
            std::string checked = formWidgetButton->getState() ? "checked=\"checked\"" : "";

            out << "<input name=\"" << formWidgetButton->getFullyQualifiedName()
                << "\" type=\"checkbox\""
                << checked
                << "data-checked=\"" << formWidgetButton->getOnStr() << "\""
                << "style=\"left: " << pos.x1 << "px; bottom: " << pos.y1 << "px;"
                << " width: " << width << "px; height: " << TO_STR(height) << "px;\" />" << endl;
        }

        static void print(ofstream &out, InputPosition &pos, SelectWidget *widget) {
            double width = pos.getWidth() + 1;
            double height = pos.getHeight() + 3;
            std::string multiple = widget->isMultiSelect() ? " multiple " : "";

            out << "<select name=\"" << widget->getFullyQualifiedName()
                << "\"" << multiple
                << "style=\"left: " << pos.x1 << "px; bottom: " << pos.y1 << "px;"
                << " width: " << width << "px; height: " << TO_STR(height) << "px;\" >" << endl;

            for (int i = 0; i < widget->getNumChoices(); i++) {
                GooString * defaultVal = widget->getOptionVal(i);
                GooString * fromNum = GooString::fromInt(i);

                out << "<option value=\"" << widget->getOptionVal(i) << "\" "
                    << (widget->isSelected(i) ? "selected" : "") << ">"
                    << (defaultVal == nullptr ? fromNum : defaultVal)
                    << "</option>";

                free(fromNum);
            }

            out << "</select>";

        }
    };

    void HTMLRenderer::process_form(ofstream &out) {
        FormPageWidgets *widgets = cur_catalog->getPage(pageNum)->getFormWidgets();
        int num = widgets->getNumWidgets();

        for (int i = 0; i < num; i++) {
            FormWidget *w = widgets->getWidget(i);
            InputPosition position(w, param.zoom);

            if (w->getType() == formText) {
                FormPrint::print(out, position, static_cast<FormWidgetText *>(w));
            } else if (w->getType() == formButton) {
                FormPrint::print(out, position, static_cast<FormWidgetButton *>(w));
            } else if (w->getType() == formChoice) {
                FormPrint::print(out, position, static_cast<SelectWidget *>(w));
            } else {
                cerr << "Unsupported form field detected" << endl;
            }
        }
    }

}
