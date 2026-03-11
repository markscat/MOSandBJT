#include "ui/mosandbjt.h"
#include "include/mosfet.h"
#include "include/bjt.h"
#include "include/curveexporter.h"
#include "include/File_save.h"

#define TEST_MOSFET
#ifdef TEST_MOSFET
#include <iostream>
#include <vector>
#include <sstream>  // 加上這行，等等要用
#else

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#endif
int main(int argc, char *argv[])
{

#define TEST_MOSFET
#ifdef TEST_MOSFET
/*
    // ========== 測試模式 ==========
    MOSFET mos;

    mos.setParameter("Vth", 2.0);
    mos.setParameter("Kn", 0.1);
    mos.setCurvePoints(2000);

    // --- 測試 output curve ---
    //輸入Vgs
    auto curve1 = mos.outputCurve(3.5);

    // 方法一：直接用 CurveExporter（簡單）
    CurveExporter::toCSV(curve1, "mosfet_output.csv", "Vds", "Id");

    // 方法二：用 File_save 產生時間戳檔名
    std::string filename1 = File_save::generateTimestampFilename("MOS_OUT_") + ".csv";
    CurveExporter::toCSV(curve1, filename1, "Vds", "Id");


    std::ofstream file("mosfet_family.csv");
    file << "Vds,Id_Vgs4V,Id_Vgs5V,Id_Vgs6V,Id_Vgs7V,Id_Vgs8V,Id_Vgs9V,Id_Vgs10V\n";

    // 取 100 個 Vds 點
    for (int j = 0; j <= 100; j++) {
        double Vds = (j / 100.0) * mos.getParameter("Vds_max");
        file << Vds;

        // 對每個 Vgs 算 Id
        for (int i = 4; i <= 10; i++) {
            double Id = mos.findIdFromVgs(i, Vds);  // 需要這個方法
            file << "," << Id;
        }
        file << "\n";
    }



    // --- 測試 transfer curve ---
    //輸入Vds
    auto curve2 = mos.transferCurve(10.0);

    // 存成 TXT
    std::string filename2 = File_save::generateTimestampFilename("MOS_TRANS_") + ".txt";
    CurveExporter::toTXT(curve2, filename2);

    // --- 測試 File_save 自己組資料 ---
    std::stringstream ss;
    ss << "MOSFET Parameters:\n";
    ss << "Vth = " << mos.getParameter("Vth") << " V\n";
    ss << "Kn  = " << mos.getParameter("Kn") << " A/V²\n";
    ss << "Curve points: " << mos.getCurvePoints() << "\n";

    std::string paramFile = File_save::generateTimestampFilename("MOS_PARAM_") + ".txt";
    File_save::overwriteFile(paramFile, ss.str());
*/

    // ========== 加入 BJT 的曲線測試 ==========
    std::cout << "\n開始測試 BJT..." << std::endl;

    BJT bjt;  // 記得要 #include "include/bjt.h"

    // 設定參數（以 2N3904 為例）
    bjt.setParameter("Beta", 100);
    bjt.setParameter("Vbe_on", 0.65);
    bjt.setParameter("Vce_sat", 0.2);
    bjt.setParameter("Va", 50);
    bjt.setCurvePoints(2000);

    // 要跑的 Ib 值（單位：A）
    std::vector<double> ib_values = {5e-6, 10e-6, 20e-6, 30e-6, 40e-6, 50e-6};

    // 方法一：用 CurveExporter 的 metadata 版（最簡單）
    std::string filename = "BJT_output_curves.csv";

    // 先取得所有曲線
    std::vector<std::vector<Point>> all_curves;
    for (double Ib : ib_values) {
        all_curves.push_back(bjt.outputCurve(Ib));
    }

    // 假設所有曲線的 Vce 點數一樣（從 outputCurve 實作來看，是的）
    int num_points = all_curves[0].size();

    // 準備 metadata
    std::stringstream metadata;
    metadata << "BJT Output Characteristics\n";
    metadata << "Beta = " << bjt.getParameter("Beta") << "\n";
    metadata << "Va = " << bjt.getParameter("Va") << " V\n";
    metadata << "Ib values: ";
    for (double Ib : ib_values) {
        metadata << Ib * 1e6 << "uA ";
    }

    // 組 CSV 內容
    std::stringstream content;
    content << "# " << metadata.str() << "\n";
    content << "Vce(V)";

    for (double Ib : ib_values) {
        content << ",Ic_Ib" << (int)(Ib * 1e6) << "uA(A)";
    }
    content << "\n";

    // 寫入資料
    for (int i = 0; i < num_points; i++) {
        content << all_curves[0][i].x;  // Vce 值（假設都一樣）

        for (size_t j = 0; j < ib_values.size(); j++) {
            content << "," << all_curves[j][i].y;  // 對應的 Ic
        }
        content << "\n";
    }

    // 用 File_save 存檔
    File_save::overwriteFile(filename, content.str());
    std::cout << "已產生: " << filename << std::endl;



    std::cout << "\n所有測試完成！" << std::endl;




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
