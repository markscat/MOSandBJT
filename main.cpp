#include "ui/mosandbjt.h"
#include "include/mosfet.h"
#include "include/curveexporter.h"

#define TEST_MOSFET
#ifdef TEST_MOSFET
#include <iostream>
#include <vector>
#else

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#endif
int main(int argc, char *argv[])
{

#define TEST_MOSFET
#ifdef TEST_MOSFET

    MOSFET mos;

    // 加入這個，編譯器會告訴你哪個沒實作
    //MOSFET* p = new MOSFET();  // 同樣會出錯，但訊息可能更清楚
    /*
D:\for work\temp\workshorp\PC\Qt\MOSandBJT\main.cpp:21: error: cannot declare variable 'mos' to be of abstract type 'MOSFET'
D:/for work/temp/workshorp/PC/Qt/MOSandBJT/main.cpp: In function 'int qMain(int, char**)':
D:/for work/temp/workshorp/PC/Qt/MOSandBJT/main.cpp:14:12: error: cannot declare variable 'mos' to be of abstract type 'MOSFET'
   14 |     MOSFET mos;
      |            ^~~
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
