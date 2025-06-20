import { $ } from '../utils.js';

export function renderFooter() {
  const footerContainer = $('#footer-container');
  footerContainer.innerHTML = `
    <footer class="bg-dark text-white py-12">
      <div class="container mx-auto px-4">
        <div class="grid grid-cols-1 md:grid-cols-3 gap-8">
          <div>
            <h3 class="text-xl font-bold mb-4">关于我</h3>
            <p class="text-gray-400">一个热爱技术和分享的开发者，记录生活和学习的点滴。</p>
          </div>
          <div>
            <h3 class="text-xl font-bold mb-4">联系方式</h3>
            <div class="flex space-x-4">
              <a href="#" class="text-gray-400 hover:text-white transition-colors">
                <i class="fa fa-github text-xl"></i>
              </a>
              <a href="#" class="text-gray-400 hover:text-white transition-colors">
                <i class="fa fa-twitter text-xl"></i>
              </a>
              <a href="#" class="text-gray-400 hover:text-white transition-colors">
                <i class="fa fa-linkedin text-xl"></i>
              </a>
            </div>
          </div>
          <div>
            <h3 class="text-xl font-bold mb-4">订阅更新</h3>
            <div class="flex">
              <input type="email" placeholder="输入你的邮箱" class="px-4 py-2 rounded-l-lg w-full focus:outline-none text-dark">
              <button class="bg-primary hover:bg-primary/90 px-4 py-2 rounded-r-lg transition-colors">
                <i class="fa fa-paper-plane"></i>
              </button>
            </div>
          </div>
        </div>
        <div class="border-t border-gray-700 mt-8 pt-8 text-center text-gray-400">
          <p>&copy; 2025 个人网站 版权所有</p>
        </div>
      </div>
    </footer>
  `;
}  