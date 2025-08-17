document.addEventListener('DOMContentLoaded', function() {
    const addTaskBtn = document.getElementById('add-task-btn');
    const taskModal = document.getElementById('task-modal');
    const closeModal = document.querySelector('.close');
    const cancelBtn = document.getElementById('cancel-btn');
    const taskForm = document.getElementById('task-form');
    const filterStatus = document.getElementById('filter-status');
    const taskList = document.getElementById('task-list');

    let tasks = [
        {
            id: 1,
            title: 'DX11 엔진 빌드 자동화',
            description: '빌드 시스템 설정 및 자동화 완료',
            status: 'done',
            date: '2024-08-17',
            priority: 'high'
        },
        {
            id: 2,
            title: '입력 시스템 구현',
            description: '키보드/마우스 입력 처리 시스템 개발',
            status: 'in-progress',
            date: '2024-08-17',
            priority: 'medium'
        },
        {
            id: 3,
            title: '렌더링 파이프라인 최적화',
            description: '성능 향상을 위한 렌더링 최적화',
            status: 'todo',
            date: '2024-08-17',
            priority: 'low'
        }
    ];

    addTaskBtn.addEventListener('click', function() {
        taskModal.style.display = 'block';
    });

    closeModal.addEventListener('click', function() {
        taskModal.style.display = 'none';
        resetForm();
    });

    cancelBtn.addEventListener('click', function() {
        taskModal.style.display = 'none';
        resetForm();
    });

    window.addEventListener('click', function(event) {
        if (event.target === taskModal) {
            taskModal.style.display = 'none';
            resetForm();
        }
    });

    taskForm.addEventListener('submit', function(e) {
        e.preventDefault();
        
        const title = document.getElementById('task-title').value;
        const description = document.getElementById('task-description').value;
        const priority = document.getElementById('task-priority').value;
        
        const newTask = {
            id: Date.now(),
            title: title,
            description: description,
            status: 'todo',
            date: new Date().toISOString().split('T')[0],
            priority: priority
        };
        
        tasks.push(newTask);
        renderTasks();
        updateStats();
        taskModal.style.display = 'none';
        resetForm();
    });

    filterStatus.addEventListener('change', function() {
        renderTasks();
    });

    function resetForm() {
        document.getElementById('task-title').value = '';
        document.getElementById('task-description').value = '';
        document.getElementById('task-priority').value = 'medium';
    }

    function renderTasks() {
        const filter = filterStatus.value;
        const filteredTasks = filter === 'all' ? tasks : tasks.filter(task => {
            if (filter === 'todo') return task.status === 'todo';
            if (filter === 'in-progress') return task.status === 'in-progress';
            if (filter === 'done') return task.status === 'done';
            return true;
        });

        taskList.innerHTML = '';
        
        filteredTasks.forEach(task => {
            const taskElement = createTaskElement(task);
            taskList.appendChild(taskElement);
        });
    }

    function createTaskElement(task) {
        const taskDiv = document.createElement('div');
        taskDiv.className = 'task-item';
        taskDiv.dataset.status = task.status;
        
        const statusText = {
            'todo': '할 일',
            'in-progress': '진행 중',
            'done': '완료'
        };
        
        taskDiv.innerHTML = `
            <div class="task-checkbox">
                <input type="checkbox" ${task.status === 'done' ? 'checked' : ''} onchange="toggleTaskStatus(${task.id})">
            </div>
            <div class="task-content">
                <h4>${task.title}</h4>
                <p>${task.description}</p>
                <span class="task-date">${task.date}</span>
            </div>
            <div class="task-status ${task.status}">${statusText[task.status]}</div>
            <button class="delete-task" onclick="deleteTask(${task.id})" style="margin-left: 1rem; background: #dc3545; color: white; border: none; padding: 0.5rem; border-radius: 5px; cursor: pointer;">삭제</button>
        `;
        
        return taskDiv;
    }

    window.toggleTaskStatus = function(taskId) {
        const task = tasks.find(t => t.id === taskId);
        if (task) {
            if (task.status === 'todo') {
                task.status = 'in-progress';
            } else if (task.status === 'in-progress') {
                task.status = 'done';
            } else {
                task.status = 'todo';
            }
            renderTasks();
            updateStats();
        }
    };

    window.deleteTask = function(taskId) {
        if (confirm('이 작업을 삭제하시겠습니까?')) {
            tasks = tasks.filter(t => t.id !== taskId);
            renderTasks();
            updateStats();
        }
    };

    function updateStats() {
        const totalTasks = tasks.length;
        const doneTasks = tasks.filter(t => t.status === 'done').length;
        const inProgressTasks = tasks.filter(t => t.status === 'in-progress').length;
        const todoTasks = tasks.filter(t => t.status === 'todo').length;
        
        const progress = totalTasks > 0 ? Math.round((doneTasks / totalTasks) * 100) : 0;
        
        document.querySelector('.progress-fill').style.width = progress + '%';
        document.querySelector('.progress-text').textContent = progress + '%';
        
        const statNumbers = document.querySelectorAll('.stat-number');
        statNumbers[0].textContent = totalTasks;
        statNumbers[1].textContent = doneTasks;
        statNumbers[2].textContent = inProgressTasks;
        statNumbers[3].textContent = todoTasks;
    }

    document.querySelectorAll('nav a').forEach(link => {
        link.addEventListener('click', function(e) {
            e.preventDefault();
            const targetId = this.getAttribute('href').substring(1);
            const targetElement = document.getElementById(targetId);
            if (targetElement) {
                targetElement.scrollIntoView({ behavior: 'smooth' });
            }
        });
    });

    document.querySelectorAll('.folder-name').forEach(folder => {
        folder.addEventListener('click', function() {
            const content = this.parentElement.querySelector('.folder-content');
            if (content.style.display === 'none') {
                content.style.display = 'block';
                this.previousElementSibling.textContent = '📂';
            } else {
                content.style.display = 'none';
                this.previousElementSibling.textContent = '📁';
            }
        });
    });

    renderTasks();
    updateStats();
});