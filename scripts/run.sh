#!/bin/bash

# 博客系统运行脚本

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
    echo "  -d, --daemon        作为守护进程运行"
    echo "  -c, --config        指定配置文件路径"
    echo "  -p, --pidfile       指定PID文件路径"
    exit 0
}

# 默认配置
CONFIG_FILE="config/config.json"
DAEMON=false
PID_FILE="/var/run/blog_system.pid"

# 解析命令行参数
while [[ $# -gt 0 ]]; do
    case $1 in
        -h|--help)
            show_help
            ;;
        -d|--daemon)
            DAEMON=true
            shift
            ;;
        -c|--config)
            if [[ -n $2 ]]; then
                CONFIG_FILE=$2
                shift 2
            else
                handle_error "请指定配置文件路径"
            fi
            ;;
        -p|--pidfile)
            if [[ -n $2 ]]; then
                PID_FILE=$2
                shift 2
            else
                handle_error "请指定PID文件路径"
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
EXECUTABLE="$BUILD_DIR/blog_system"

# 检查可执行文件是否存在
if [ ! -f "$EXECUTABLE" ]; then
    handle_error "可执行文件不存在，请先构建项目"
fi

# 检查配置文件是否存在
if [ ! -f "$PROJECT_ROOT/$CONFIG_FILE" ]; then
    handle_error "配置文件不存在: $CONFIG_FILE"
fi

# 检查是否已在运行
if [ -f "$PID_FILE" ]; then
    PID=$(cat "$PID_FILE")
    if ps -p "$PID" > /dev/null; then
        handle_error "博客系统已在运行 (PID: $PID)"
    else
        echo -e "${YELLOW}警告: 发现旧的PID文件，正在删除...${NC}"
        rm -f "$PID_FILE"
    fi
fi

# 运行应用程序
echo -e "${YELLOW}启动博客系统...${NC}"
echo -e "${YELLOW}使用配置文件: ${PROJECT_ROOT}/${CONFIG_FILE}${NC}"

if [ "$DAEMON" = true ]; then
    cd "$BUILD_DIR" || handle_error "无法进入构建目录"
    nohup "./blog_system" --config "$PROJECT_ROOT/$CONFIG_FILE" > /dev/null 2>&1 &
    echo $! > "$PID_FILE"
    echo -e "${GREEN}博客系统已作为守护进程启动 (PID: $(cat "$PID_FILE"))${NC}"
else
    cd "$BUILD_DIR" || handle_error "无法进入构建目录"
    "./blog_system" --config "$PROJECT_ROOT/$CONFIG_FILE"
fi