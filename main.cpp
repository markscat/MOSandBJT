#include "ui/mosandbjt.h"
#include "include/mosfet.h"
#include "include/curveexporter.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>

int main(int argc, char *argv[])
{

#define TEST_MOSFET
#ifdef TEST_MOSFET
    MOSFET mos;
    /*
     * main.cpp:12:12: Variable type 'MOSFET' is an abstract class
       transistor.h:34:30: unimplemented pure virtual method 'transferCurve' in 'MOSFET'
*/
    mos.setParameter("Vth", 2.0);
    mos.setParameter("Kn", 0.1);

    // 算曲線
    auto curve = mos.outputCurve(3.5);

    // 存成 CSV
    CurveExporter::toCSV(curve, "mosfet_curve.csv");

    // 也可以存成 TXT
    CurveExporter::toTXT(curve, "mosfet_curve.txt");

    return 0;

    #else



    QApplication a(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "MOSandBJT_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }
    MOSandBJT w;
    w.show();
    return a.exec();

#endif

}
