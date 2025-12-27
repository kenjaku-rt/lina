{ pkgs ? import <nixpkgs> {} }:

pkgs.mkShell {
  nativeBuildInputs = with pkgs; [
    cmake        # Добавляем CMake сюда
    pkg-config
    gcc
    gnumake
  ];

  buildInputs = with pkgs; [
    # Используем полную версию Python с заголовками
    (python3.withPackages (ps: with ps; [ ])) 
    julia-bin
  ];

  shellHook = ''
    # Экспортируем пути, чтобы CMake не гадал
    export PYTHON_INCLUDE_DIR="${pkgs.python3}/include/python3.12"
    export PYTHON_LIBRARY="${pkgs.python3}/lib/libpython3.12.so"
    
    export JULIA_INCLUDE_DIR="${pkgs.julia-bin}/include/julia"
    export JULIA_LIB_DIR="${pkgs.julia-bin}/lib"

    echo "✅ Lina Environment Ready! Python & Julia paths exported."
  '';
}
