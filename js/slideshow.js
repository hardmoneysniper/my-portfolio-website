document.addEventListener('DOMContentLoaded', () => {
    const slides = document.querySelectorAll('.slide');
    const detailsButton = document.querySelector('.details-button');
    const descriptionBox = document.querySelector('.image-description');
    let currentIndex = 0;
    let slideInterval = null;
    const intervalTime = 3500; // 5 seconds per slide

    function showSlide(index) {
        slides.forEach(s => s.classList.remove('active'));
        slides[index].classList.add('active');

        // Update details description and link
        const desc = slides[index].getAttribute('data-description') || '';
        const link = slides[index].getAttribute('data-link') || '#';
        descriptionBox.textContent = desc;
        detailsButton.setAttribute('href', link);
    }

    function nextSlide() {
        currentIndex = (currentIndex + 1) % slides.length;
        showSlide(currentIndex);
    }

    function startSlideshow() {
        if (!slideInterval) {
            slideInterval = setInterval(nextSlide, intervalTime);
        }
    }

    function stopSlideshow() {
        if (slideInterval) {
            clearInterval(slideInterval);
            slideInterval = null;
        }
    }

    // Hover events to show overlay and stop slideshow
    detailsButton.addEventListener('mouseenter', () => {
        document.body.classList.add('hovering-details');
        stopSlideshow();
        detailsButton.textContent = "Go to This Project"; // Change text on hover
    });

    detailsButton.addEventListener('mouseleave', () => {
        document.body.classList.remove('hovering-details');
        startSlideshow();
        detailsButton.textContent = "About This Project"; // Revert to original text
    });

    // Initial setup
    showSlide(currentIndex);
    startSlideshow();
});

const desc = slides[index].getAttribute('data-description') || '';
descriptionBox.textContent = desc;

const detailsButton = document.querySelector('.details-button');
let originalText = detailsButton.textContent; // store original text