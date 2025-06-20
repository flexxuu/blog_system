import { $, $$, formatDate, fetchData } from '../utils.js';
import { base_url } from '../config.js';

// 渲染博客相关UI组件
export function renderBlogComponents() {
  renderLatestBlogsContainer();
  renderBlogFilter();
  renderBlogCardsContainer();
}

// 渲染最新博客容器
function renderLatestBlogsContainer() {
  const container = $('#latest-blogs-container');
  container.innerHTML = `
    <h2 class="text-2xl font-bold mb-6 flex items-center">
      <i class="fa fa-newspaper-o text-primary mr-2"></i>最新博客
    </h2>
    <div class="space-y-6" id="home-blogs">
      <div class="bg-white p-6 rounded-lg card-shadow hover-scale">
        <h3 class="text-xl font-bold mb-2">加载中...</h3>
        <p class="text-gray-600">请稍候，正在加载博客内容...</p>
      </div>
    </div>
  `;
}

// 渲染博客筛选器
function renderBlogFilter() {
  const container = $('#blog-filter-container');
  container.innerHTML = `
    <div class="flex flex-col md:flex-row justify-between items-start md:items-center mb-8 gap-4">
      <div class="flex flex-wrap gap-2">
        <button class="category-btn bg-primary text-white px-4 py-2 rounded-full text-sm">全部</button>
        <button class="category-btn bg-white text-dark hover:bg-gray-100 px-4 py-2 rounded-full text-sm transition-colors">技术</button>
        <button class="category-btn bg-white text-dark hover:bg-gray-100 px-4 py-2 rounded-full text-sm transition-colors">生活</button>
        <button class="category-btn bg-white text-dark hover:bg-gray-100 px-4 py-2 rounded-full text-sm transition-colors">学习</button>
      </div>
      <div class="relative w-full md:w-auto">
        <input type="text" id="blog-search" placeholder="搜索博客..." class="pl-10 pr-4 py-2 border border-gray-300 rounded-full focus:outline-none focus:ring-2 focus:ring-primary/50 w-full md:w-64">
        <i class="fa fa-search absolute left-4 top-1/2 transform -translate-y-1/2 text-gray-400"></i>
      </div>
    </div>
  `;

  // 博客筛选
  $$('.category-btn').forEach(button => {
    button.addEventListener('click', () => {
      // 移除所有按钮的活跃状态
      $$('.category-btn').forEach(btn => {
        btn.classList.remove('bg-primary', 'text-white');
        btn.classList.add('bg-white', 'text-dark', 'hover:bg-gray-100');
      });
      
      // 设置当前按钮的活跃状态
      button.classList.remove('bg-white', 'text-dark', 'hover:bg-gray-100');
      button.classList.add('bg-primary', 'text-white');
      
      const category = button.textContent === '全部' ? '' : button.textContent;
      const search = $('#blog-search').value;
      
      loadBlogs(category, search);
    });
  });
  
  // 博客搜索
  $('#blog-search').addEventListener('input', debounce(async (event) => {
    const search = event.target.value;
    const activeCategoryBtn = $('.category-btn.bg-primary');
    const category = activeCategoryBtn.textContent === '全部' ? '' : activeCategoryBtn.textContent;
    
    loadBlogs(category, search);
  }, 300));
}

// 渲染博客卡片容器
function renderBlogCardsContainer() {
  const container = $('#blog-cards-container');
  container.innerHTML = `
    <div class="grid grid-cols-1 md:grid-cols-2 lg:grid-cols-3 gap-8" id="blog-cards">
      <div class="bg-white p-6 rounded-lg card-shadow hover-scale">
        <h3 class="text-xl font-bold mb-2">加载中...</h3>
        <p class="text-gray-600">请稍候，正在加载博客内容...</p>
      </div>
    </div>
  `;
}

// 加载最新博客
export async function loadLatestBlogs() {
  try {
    const response = await fetchData(`${base_url}/api/blogs?limit=3`);
    
    if (response.success && Array.isArray(response.data) && response.data.length > 0) {
      const container = $('#home-blogs');
      container.innerHTML = '';
      
      response.data.forEach(blog => {
        const blogElement = document.createElement('div');
        blogElement.innerHTML = `
          <div class="bg-white p-6 rounded-lg card-shadow hover-scale">
            <h3 class="text-xl font-bold mb-2">${blog.title}</h3>
            <p class="text-gray-600">${blog.summary}</p>
            <button class="mt-4 inline-block text-primary hover:underline view-blog-btn" data-id="${blog.id}">阅读更多</button>
          </div>
        `;
        container.appendChild(blogElement);
      });
      
      // 为阅读更多按钮添加事件监听
      $$('.view-blog-btn').forEach(btn => {
        btn.addEventListener('click', () => {
          const blogId = btn.getAttribute('data-id');
          openBlogDetails(blogId);
        });
      });
    } else {
      $('#home-blogs').innerHTML = `
        <div class="bg-white p-6 rounded-lg card-shadow">
          <p class="text-gray-500">暂无博客内容</p>
        </div>
      `;
    }
  } catch (error) {
    console.error('加载最新博客失败:', error);
    $('#home-blogs').innerHTML = `
      <div class="bg-white p-6 rounded-lg card-shadow">
        <p class="text-red-500">加载博客失败，请稍后再试</p>
      </div>
    `;
  }
}

// 加载博客列表
export async function loadBlogs(category = '', search = '') {
  try {
    let url = `${base_url}/api/blogs`;
    const params = [];
    
    if (category) params.push(`category=${encodeURIComponent(category)}`);
    if (search) params.push(`search=${encodeURIComponent(search)}`);
    
    if (params.length > 0) {
      url += '?' + params.join('&');
    }
    
    const response = await fetchData(url);
    
    if (response.success && Array.isArray(response.data) && response.data.length > 0) {
      const container = $('#blog-cards');
      container.innerHTML = '';
      
      response.data.forEach(blog => {
        const blogElement = document.createElement('div');
        blogElement.innerHTML = `
          <div class="bg-white p-6 rounded-lg card-shadow hover-scale">
            <h3 class="text-xl font-bold mb-2">${blog.title}</h3>
            <p class="text-gray-600">${blog.summary}</p>
            <button class="mt-4 inline-block text-primary hover:underline view-blog-btn" data-id="${blog.id}">阅读更多</button>
          </div>
        `;
        container.appendChild(blogElement);
      });
      
      // 为阅读更多按钮添加事件监听
      $$('.view-blog-btn').forEach(btn => {
        btn.addEventListener('click', () => {
          const blogId = btn.getAttribute('data-id');
          openBlogDetails(blogId);
        });
      });
    } else {
      $('#blog-cards').innerHTML = `
        <div class="bg-white p-6 rounded-lg card-shadow col-span-full text-center">
          <p class="text-gray-500">没有找到匹配的博客</p>
        </div>
      `;
    }
  } catch (error) {
    console.error('加载博客失败:', error);
    $('#blog-cards').innerHTML = `
      <div class="bg-white p-6 rounded-lg card-shadow col-span-full text-center">
        <p class="text-red-500">加载博客失败，请稍后再试</p>
      </div>
    `;
  }
}

// 打开博客详情
export async function openBlogDetails(blogId) {
  try {
    const response = await fetchData(`${base_url}/api/blogs/${blogId}`);
    
    if (response.success && response.data) {
      const blog = response.data;
      $('#modal-title').textContent = blog.title;
      $('#modal-date').textContent = formatDate(blog.createdAt);
      $('#modal-category').textContent = blog.category;
      $('#modal-content').textContent = blog.content;
      
      // 设置编辑博客按钮的事件
      $('#edit-blog').setAttribute('data-id', blogId);
      
      // 设置删除博客按钮的事件
      $('#delete-blog').setAttribute('data-id', blogId);
      
      // 显示博客详情模态框
      $('#blog-modal').classList.remove('hidden');
    }
  } catch (error) {
    console.error('加载博客详情失败:', error);
    alert('加载博客详情失败，请稍后再试');
  }
}

// 编辑博客
export function setupBlogEdit() {
  $('#edit-blog').addEventListener('click', () => {
    const blogId = $('#edit-blog').getAttribute('data-id');
    
    if (blogId) {
      // 编辑现有博客
      $('#edit-blog-modal-title').textContent = '编辑博客';
      $('#save-edit-blog').setAttribute('data-id', blogId);
      
      // 加载博客数据到表单中
      loadBlogForEdit(blogId);
      
      // 显示编辑模态框
      $('#edit-blog-modal').classList.remove('hidden');
    }
  });
}

// 加载博客数据到编辑表单
async function loadBlogForEdit(blogId) {
  try {
    const response = await fetchData(`${base_url}/api/blogs/${blogId}`);
    
    if (response.success && response.data) {
      const blog = response.data;
      $('#edit-blog-title').value = blog.title;
      $('#edit-blog-category').value = blog.category;
      $('#edit-blog-summary').value = blog.summary;
      $('#edit-blog-content').value = blog.content;
    }
  } catch (error) {
    console.error('加载博客数据失败:', error);
    alert('加载博客