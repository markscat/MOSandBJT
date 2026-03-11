/**
MOSandBJT/
├── core/                      # 核心演算法（不依賴 Qt）
│   ├── models/                # 電晶體模型
│   │   ├── transistor.h       # 抽象基底類別
│   │   ├── bjt.h              # BJT 模型宣告
│   │   ├── bjt.cpp            # BJT 模型實作
│   │   ├── mosfet.h           # MOSFET 模型宣告
│   │   └── mosfet.cpp         # MOSFET 模型實作
│   │
│   ├── math/                  # 數學工具(整合到MOSFET以及BJT中)
│   │   ├── curves.h           # 曲線計算
│   │   ├── interpolation.h    # 內插（找點用）
│   │   └── solver.h           # 解方程（找工作點）
│   │
│   └── parameters/            # 參數處理(整合到MOSFET以及BJT中)
│       ├── parameter.h        # 參數定義（名稱、範圍、單位）
│       └── validator.h        # 參數驗證（合理範圍檢查）
│
├── data/                      # 資料檔（非程式碼）
│   ├── parameters/            # 預設參數檔（JSON）
│   │   ├── bjt_2n3904.json
│   │   ├── bjt_2n2222.json
│   │   ├── mos_2n7000.json
│   │   └── mos_irf540.json
│   │
│   └── curves/                # 可選：預先算好的曲線點（加速用）
│
├── ui/                        # Qt 介面（依賴 core）
│   ├── widgets/               # 自訂元件
│   │   ├── parameterpanel.h   # 左邊參數面板
│   │   ├── parameterpanel.cpp
│   │   ├── plotwidget.h       # 右邊繪圖區
│   │   └── plotwidget.cpp
│   │
│   ├── dialogs/               # 對話框
│   │   ├── findpointdialog.h  # 找點對話框（輸入 x 或 y）
│   │   └── findpointdialog.cpp
│   │
│   ├── mainwindow.h           # 主視窗
│   ├── mainwindow.cpp
│   └── mainwindow.ui
│
├── resources/                 # 資源檔（圖示、翻譯等）
│
├── tests/                     # 測試（可選）
│   ├── test_bjt.cpp
│   └── test_mosfet.cpp
│
├── docs/                      # 文件
│   └── parameters.md          # 參數說明（給使用者看）
│
├── MOSandBJT.pro               # Qt 專案檔
├── README.md                   # 專案說明
└── .gitignore                  # Git 忽略檔
*/