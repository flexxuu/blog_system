import { $ } from '../utils.js';

export function renderModals() {
  const modalsContainer = $('#modals-container');
  modalsContainer.innerHTML = `
    <!-- 博客详情模态框 -->
    <div id="blog-modal" class="fixed inset-0 bg-black/50 z-50 hidden flex items-center justify-center p-4">
      <div class="bg-white rounded-xl max-w-4xl w-full max-h-[90vh] overflow-y-auto">
        <div class="p-6">
          <div class="flex justify-between items-start mb-4">
            <h3 id="modal-title" class="text-2xl font-bold"></h3>
            <div class="flex space-x-2">
              <button id="edit-blog" class="text-primary hover:text-primary/80">
                <i class="fa fa-pencil"></i> 编辑
              </button>
              <button id="close-modal" class="text-gray-400 hover:text-gray-600">
                <i class="fa fa-times text-xl"></i>
              </button>
            </div>
          </div>
          <div class="text-gray-500 mb-4 flex items-center">
            <span id="modal-date" class="mr-4"></span>
            <span id="modal-category"></span>
          </div>
          <div id="modal-content" class="prose max-w-none"></div>
          <div class="mt-6 pt-4 border-t border-gray-100">
            <button id="delete-blog" class="text-red-500 hover:text-red-600 font-medium flex items-center">
              <i class="fa fa-trash mr-2"></i> 删除博客
            </button>
          </div>
        </div>
      </div>
    </div>
    
    <!-- 新建/编辑博客模态框 -->
    <div id="edit-blog-modal" class="fixed inset-0 bg-black/50 z-50 hidden flex items-center justify-center p-4">
      <div class="bg-white rounded-xl max-w-4xl w-full max-h-[90vh] overflow-y-auto">
        <div class="p-6">
          <div class="flex justify-between items-center mb-4">
            <h3 id="edit-blog-modal-title" class="text-xl font-bold">新建博客</h3>
            <button id="close-edit-blog-modal" class="text-gray-400 hover:text-gray-600">
              <i class="fa fa-times"></i>
            </button>
          </div>
          <div class="space-y-4">
            <div>
              <label for="edit-blog-title" class="block text-sm font-medium text-gray-700 mb-1">标题</label>
              <input type="text" id="edit-blog-title" class="w-full px-4 py-3 border border-gray-300 rounded-lg focus:outline-none focus:ring-2 focus:ring-primary/50" placeholder="输入博客标题">
            </div>
            
            <div>
              <label for="edit-blog-category" class="block text-sm font-medium text-gray-700 mb-1">分类</label>
              <select id="edit-blog-category" class="w-full px-4 py-3 border border-gray-300 rounded-lg focus:outline-none focus:ring-2 focus:ring-primary/50">
                <option value="技术">技术</option>
                <option value="生活">生活</option>
                <option value="学习">学习</option>
              </select>
            </div>
            
            <div>
              <label for="edit-blog-summary" class="block text-sm font-medium text-gray-700 mb-1">摘要</label>
              <textarea id="edit-blog-summary" rows="3" class="w-full px-4 py-3 border border-gray-300 rounded-lg focus:outline-none focus:ring-2 focus:ring-primary/50" placeholder="输入博客摘要（简短描述）"></textarea>
            </div>
            
            <div>
              <label for="edit-blog-content" class="block text-sm font-medium text-gray-700 mb-1">内容</label>
              <div class="border border-gray-300 rounded-lg overflow-hidden">
                <div class="editor-toolbar bg-gray-50 p-2 flex flex-wrap gap-2">
                  <button type="button" data-format="bold" title="粗体"><i class="fa fa-bold"></i></button>
                  <button type="button" data-format="italic" title="斜体"><i class="fa fa-italic"></i></button>
                  <button type="button" data-format="underline" title="下划线"><i class="fa fa-underline"></i></button>
                  <button type="button" data-format="link" title="链接"><i class="fa fa-link"></i></button>
                  <button type="button" data-format="header" data-value="1" title="标题1"><i class="fa fa-header"><sup>1</sup></i></button>
                  <button type="button" data-format="header" data-value="2" title="标题2"><i class="fa fa-header"><sup>2</sup></i></button>
                  <button type="button" data-format="list" data-value="ul" title="无序列表"><i class="fa fa-list-ul"></i></button>
                  <button type="button" data-format="list" data-value="ol" title="有序列表"><i class="fa fa-list-ol"></i></button>
                  <button type="button" data-format="code" title="代码块"><i class="fa fa-code"></i></button>
                </div>
                <textarea id="edit-blog-content" rows="12" class="w-full px-4 py-3 border-t border-gray-200 focus:outline-none" placeholder="开始撰写你的博客内容..."></textarea>
              </div>
            </div>
            
            <div class="flex justify-end space-x-3">
              <button id="cancel-edit-blog" class="px-6 py-3 border border-gray-300 rounded-lg hover:bg-gray-50 transition-colors flex items-center">
                取消
              </button>
              <button id="save-edit-blog" class="px-6 py-3 bg-primary text-white rounded-lg hover:bg-primary/90 transition-colors flex items-center">
                <i class="fa fa-save mr-2"></i> 保存博客
              </button>
            </div>
          </div>
        </div>
      </div>
    </div>
    
    <!-- 新建/编辑备忘录模态框 -->
    <div id="memo-modal" class="fixed inset-0 bg-black/50 z-50 hidden flex items-center justify-center p-4">
      <div class="bg-white rounded-xl max-w-2xl w-full">
        <div class="p-6">
          <div class="flex justify-between items-center mb-4">
            <h3 id="memo-modal-title" class="text-xl font-bold">新建备忘录</h3>
            <button id="close-memo-modal" class="text-gray-400 hover:text-gray-600">
              <i class="fa fa-times"></i>
            </button>
          </div>
          <div class="space-y-4">
            <div>
              <label for="memo-title" class="block text-sm font-medium text-gray-700 mb-1">标题</label>
              <input type="text" id="memo-title" class="w-full px-4 py-2 border border-gray-300 rounded-lg focus:outline-none focus:ring-2 focus:ring-primary/50" placeholder="输入备忘录标题">
            </div>
            <div>
              <label for="memo-content" class="block text-sm font-medium text-gray-700 mb-1">内容</label>
              <textarea id="memo-content" rows="6" class="w-full px-4 py-2 border border-gray-300 rounded-lg focus:outline-none focus:ring-2 focus:ring-primary/50" placeholder="输入备忘录内容"></textarea>
            </div>
            <div class="flex justify-end space-x-3">
              <button id="cancel-memo" class="px-4 py-2 border border-gray-300 rounded-lg hover:bg-gray-50 transition-colors">取消</button>
              <button id="save-memo" class="px-4 py-2 bg-secondary text-white rounded-lg hover:bg-secondary/90 transition-colors">保存</button>
            </div>
          </div>
        </div>
      </div>
    </div>
  `;

  // 关闭模态框事件
  setupModalCloseEvents();
}

function setupModalCloseEvents() {
  // 博客详情模态框
  const blogModal = $('#blog-modal');
  const closeModal = $('#close-modal');
  
  closeModal.addEventListener('click', () => {
    blogModal.classList.add('hidden');
  });
  
  blogModal.addEventListener('click', (e) => {
    if (e.target === blogModal) {
      blogModal.classList.add('hidden');
    }
  });
  
  // 新建/编辑博客模态框
  const editBlogModal = $('#edit-blog-modal');
  const closeEditBlogModal = $('#close-edit-blog-modal');
  const cancelEditBlog = $('#cancel-edit-blog');
  
  closeEditBlogModal.addEventListener('click', () => {
    editBlogModal.classList.add('hidden');
  });
  
  cancelEditBlog.addEventListener('click', () => {
    editBlogModal.classList.add('hidden');
  });
  
  editBlogModal.addEventListener('click', (e) => {
    if (e.target === editBlogModal) {
      editBlogModal.classList.add('hidden');
    }
  });
  
  // 新建/编辑备忘录模态框
  const memoModal = $('#memo-modal');
  const closeMemoModal = $('#close-memo-modal');
  const cancelMemo = $('#cancel-memo');
  
  closeMemoModal.addEventListener('click', () => {
    memoModal.classList.add('hidden');
  });
  
  cancelMemo.addEventListener('click', () => {
    memoModal.classList.add('hidden');
  });
  
  memoModal.addEventListener('click', (e) => {
    if (e.target === memoModal) {
      memoModal.classList.add('hidden');
    }
  });
}  