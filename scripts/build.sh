#!/bin/bash

# 博客系统构建脚本

# 颜色定义
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[0;33m'
NC='\033[0m' # No Color

# 错误处理函数
handle_error() {
    echo -e "${RED}Error: $1${NC}"
    exit 1
}

# 显示帮助信息
show_help() {
    echo "Usage: $0 [options]"
    echo "Options:"
    echo "  -h, --help          显示此帮助信息"
    echo "  -d, --debug         构建调试版本"
    echo "  -r, --release       构建发布版本 (默认)"
    echo "  -c, --clean         清理构建目录"
    echo "  -j, --jobs          指定并行编译的线程数"
    exit 0
}

# 默认配置
BUILD_TYPE="Release"
JOBS=$(nproc)
CLEAN=false

# 解析命令行参数
while [[ $# -gt 0 ]]; do
    case $1 in
        -h|--help)
            show_help
            ;;
        -d|--debug)
            BUILD_TYPE="Debug"
            shift
            ;;
        -r|--release)
            BUILD_TYPE="Release"
            shift
            ;;
        -c|--clean)
            CLEAN=true
            shift
            ;;
        -j|--jobs)
            if [[ -n $2 && $2 =~ ^[0-9]+$ ]]; then
                JOBS=$2
                shift 2
            else
                handle_error "请指定有效的线程数"
            fi
            ;;
        *)
            handle_error "未知参数: $1"
            ;;
    esac
done

# 项目根目录
PROJECT_ROOT=$(dirname "$(realpath "$0")")/..
BUILD_DIR="$PROJECT_ROOT/backend/build"

# 清理构建目录
if [ "$CLEAN" = true ]; then
    echo -e "${YELLOW}清理构建目录...${NC}"
    rm -rf "$BUILD_DIR"
    echo -e "${GREEN}清理完成${NC}"
    exit 0
fi

# 创建构建目录
mkdir -p "$BUILD_DIR" || handle_error "无法创建构建目录"
cd "$BUILD_DIR" || handle_error "无法进入构建目录"

# 运行CMake配置
echo -e "${YELLOW}配置项目 (${BUILD_TYPE})...${NC}"
cmake -DCMAKE_BUILD_TYPE="$BUILD_TYPE" .. || handle_error "CMake配置失败"

# 编译项目
echo -e "${YELLOW}开始编译 (使用 $JOBS 个线程)...${NC}"
make -j"$JOBS" || handle_error "编译失败"

echo -e "${GREEN}构建成功!${NC}"
echo -e "${YELLOW}可执行文件位于: ${BUILD_DIR}/blog_system${NC}"