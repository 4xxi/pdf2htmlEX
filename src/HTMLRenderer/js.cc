#include <iostream>
#include <sstream>
#include <Link.h>
#include "HTMLRenderer.h"
#include "util/namespace.h"

namespace pdf2htmlEX {
    void HTMLRenderer::process_js(ofstream &out) {
        Page * page = cur_catalog->getPage(pageNum);
        Form * form = cur_catalog->getForm();

        for (int i = 0; i < form->getNumFields(); i++) {
            FormField * formField = form->getRootField(i);
            for (int j = 0; j < formField->getNumWidgets(); j++) {
                FormWidget * widget = formField->getWidget(j);
                process_js_link(widget, widget->getActivationAction());
                process_js_link(widget, widget->getAdditionalAction(Annot::actionFieldModified));
                process_js_link(widget, widget->getAdditionalAction(Annot::actionCalculateField));
            }
        }

    }

    /**
     * @param linkAction
     */
    void HTMLRenderer::process_js_link(FormWidget * widget, LinkAction * linkAction) {
        if (nullptr == linkAction) {
            return;
        }

        if (linkAction->getKind() == actionJavaScript) {
            LinkJavaScript * linkjs = static_cast<LinkJavaScript *>(linkAction);

            std::cout << widget->getType() << " " << widget->getFullyQualifiedName()->getCString() << endl;
            std::cout << linkjs->getScript()->getCString() << std::endl;
        }
    }
}

