#!/bin/bash

# 博客系统部署脚本

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
    echo "  -e, --env           指定部署环境 (development|production)"
    echo "  -c, --clean         清理并重新构建"
    exit 0
}

# 默认配置
ENV="production"
CLEAN=false

# 解析命令行参数
while [[ $# -gt 0 ]]; do
    case $1 in
        -h|--help)
            show_help
            ;;
        -e|--env)
            if [[ -n $2 && ($2 == "development" || $2 == "production") ]]; then
                ENV=$2
                shift 2
            else
                handle_error "请指定有效的环境 (development|production)"
            fi
            ;;
        -c|--clean)
            CLEAN=true
            shift
            ;;
        *)
            handle_error "未知参数: $1"
            ;;
    esac
done

# 项目根目录
PROJECT_ROOT=$(dirname "$(realpath "$0")")/..

# 部署目录
if [ "$ENV" = "production" ]; then
    DEPLOY_DIR="/opt/blog_system"
else
    DEPLOY_DIR="$PROJECT_ROOT/deploy_dev"
fi

# 创建部署目录
mkdir -p "$DEPLOY_DIR" || handle_error "无法创建部署目录"

# 构建项目
echo -e "${YELLOW}开始构建项目...${NC}"
if [ "$CLEAN" = true ]; then
    "$PROJECT_ROOT/scripts/build.sh" --clean || handle_error "清理构建失败"
fi
"$PROJECT_ROOT/scripts/build.sh" || handle_error "构建失败"

# 复制文件到部署目录
echo -e "${YELLOW}复制文件到部署目录...${NC}"

# 创建必要的目录结构
mkdir -p "$DEPLOY_DIR/bin"
mkdir -p "$DEPLOY_DIR/config"
mkdir -p "$DEPLOY_DIR/data"
mkdir -p "$DEPLOY_DIR/logs"
mkdir -p "$DEPLOY_DIR/www"

# 复制可执行文件
cp "$PROJECT_ROOT/backend/build/blog_system" "$DEPLOY_DIR/bin/" || handle_error "复制可执行文件失败"

# 复制配置文件
cp "$PROJECT_ROOT/config/config.json" "$DEPLOY_DIR/config/" || handle_error "复制配置文件失败"

# 复制静态文件
cp -r "$PROJECT_ROOT/www/." "$DEPLOY_DIR/www/" || handle_error "复制静态文件失败"

# 复制脚本
cp "$PROJECT_ROOT/scripts/run.sh" "$DEPLOY_DIR/" || handle_error "复制运行脚本失败"
chmod +x "$DEPLOY_DIR/run.sh"

# 设置权限
chmod +x "$DEPLOY_DIR/bin/blog_system"

# 创建systemd服务文件(仅生产环境)
if [ "$ENV" = "production" ]; then
    echo -e "${YELLOW}创建systemd服务文件...${NC}"
    SYSTEMD_SERVICE="/etc/systemd/system/blog_system.service"
    
    cat > "$SYSTEMD_SERVICE" << EOF
[Unit]
Description=Blog System
After=network.target

[Service]
Type=simple
User=blog
Group=blog
WorkingDirectory=$DEPLOY_DIR
ExecStart=$DEPLOY_DIR/bin/blog_system --config $DEPLOY_DIR/config/config.json
Restart=always
RestartSec=5

[Install]
WantedBy=multi-user.target
EOF
    
    echo -e "${YELLOW}重载systemd管理器配置...${NC}"
    systemctl daemon-reload || handle_error "重载systemd配置失败"
    
    echo -e "${YELLOW}启用并启动服务...${NC}"
    systemctl enable blog_system || handle_error "启用服务失败"
    systemctl start blog_system || handle_error "启动服务失败"
    
    echo -e "${GREEN}服务状态:${NC}"
    systemctl status blog_system --no-pager
fi

echo -e "${GREEN}部署成功!${NC}"
echo -e "${YELLOW}应用已部署到: ${DEPLOY_DIR}${NC}"