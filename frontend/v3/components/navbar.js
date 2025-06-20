import { $ } from '../utils.js';

export function renderNavbar() {
  const navbarContainer = $('#navbar-container');
  navbarContainer.innerHTML = `
    <nav id="navbar" class="bg-white shadow-md fixed w-full z-50 transition-all duration-300">
      <div class="container mx-auto px-4 py-3 flex justify-between items-center">
        <a href="#" class="text-2xl font-bold text-primary flex items-center">
          <i class="fa fa-user-circle mr-2"></i>
          <span>个人网站</span>
        </a>
        
        <!-- 桌面导航 -->
        <div class="hidden md:flex space-x-8">
          <a href="#home" class="nav-link font-medium hover:text-primary transition-colors">首页</a>
          <a href="#blog" class="nav-link font-medium hover:text-primary transition-colors">博客</a>
          <a href="#memo" class="nav-link font-medium hover:text-primary transition-colors">备忘录</a>
          <a href="write.html" class="nav-link font-medium hover:text-primary transition-colors">写博客</a>
        </div>
        
        <!-- 移动端菜单按钮 -->
        <button id="menuBtn" class="md:hidden text-dark focus:outline-none">
          <i class="fa fa-bars text-xl"></i>
        </button>
      </div>
      
      <!-- 移动端导航菜单 -->
      <div id="mobileMenu" class="hidden md:hidden bg-white shadow-lg absolute w-full">
        <div class="container mx-auto px-4 py-3 flex flex-col space-y-4">
          <a href="#home" class="mobile-nav-link py-2 font-medium hover:text-primary transition-colors">首页</a>
          <a href="#blog" class="mobile-nav-link py-2 font-medium hover:text-primary transition-colors">博客</a>
          <a href="#memo" class="mobile-nav-link py-2 font-medium hover:text-primary transition-colors">备忘录</a>
          <a href="write.html" class="mobile-nav-link py-2 font-medium hover:text-primary transition-colors">写博客</a>
        </div>
      </div>
    </nav>
  `;

  // 移动端菜单切换
  $('#menuBtn').addEventListener('click', () => {
    $('#mobileMenu').classList.toggle('hidden');
  });

  // 平滑滚动
  $$('a[href^="#"]').forEach(anchor => {
    anchor.addEventListener('click', function (e) {
      e.preventDefault();
      const targetId = this.getAttribute('href');
      const targetElement = $(targetId);
      
      if (targetElement) {
        window.scrollTo({
          top: targetElement.offsetTop - 80,
          behavior: 'smooth'
        });
        
        // 如果是移动端，点击导航后关闭菜单
        const mobileMenu = $('#mobileMenu');
        if (!mobileMenu.classList.contains('hidden')) {
          mobileMenu.classList.add('hidden');
        }
      }
    });
  });

  // 导航栏滚动效果
  window.addEventListener('scroll', () => {
    const navbar = $('#navbar');
    if (window.scrollY > 50) {
      navbar.classList.add('py-2', 'shadow-lg');
      navbar.classList.remove('py-3', 'shadow-md');
    } else {
      navbar.classList.add('py-3', 'shadow-md');
      navbar.classList.remove('py-2', 'shadow-lg');
    }
  });
}  