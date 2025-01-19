const viewWindow = document.querySelector('.view-window');
let isDragging = false;
let startX, scrollLeft;

viewWindow.addEventListener('mousedown', (e) => {
    isDragging = true;
    viewWindow.classList.add('active');
    startX = e.pageX - viewWindow.offsetLeft; // Mouse starting X position
    scrollLeft = viewWindow.scrollLeft; // Store initial scroll position
});

viewWindow.addEventListener('mouseleave', () => {
    isDragging = false; // Stop dragging if mouse leaves the container
    viewWindow.classList.remove('active');
});

viewWindow.addEventListener('mouseup', () => {
    isDragging = false; // Stop dragging on mouse release
    viewWindow.classList.remove('active');
});

viewWindow.addEventListener('mousemove', (e) => {
    if (!isDragging) return; // Exit if not dragging
    e.preventDefault();
    const x = e.pageX - viewWindow.offsetLeft; // Current X position
    const distance = x - startX; // Distance moved by the mouse
    viewWindow.scrollLeft = scrollLeft - distance; // Update scroll position
});