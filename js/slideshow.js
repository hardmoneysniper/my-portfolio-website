document.addEventListener('DOMContentLoaded', () => {
    const slides = document.querySelectorAll('.slide');
    const lmTitle = document.querySelector('.lm-title');
    const lmInfo = document.querySelector('.lm-info');
    const lmDescription = document.querySelector('.lm-description');
    const lmLink = document.querySelector('.lm-link');
    const panel = document.querySelector('.learn-more-panel');
    const panelBar = document.querySelector('.panel-bar');

    let currentIndex = 0;
    let slideInterval = null;
    let resumeTimeout = null;
    let isExpanded = false;
    const intervalTime = 2750;

    function showSlide(index) {
        slides.forEach(s => s.classList.remove('active'));
        slides[index].classList.add('active');
        if (lmTitle) lmTitle.textContent = slides[index].dataset.title || '';
        if (lmInfo) lmInfo.textContent = slides[index].dataset.info || '';
        if (lmDescription) lmDescription.textContent = slides[index].dataset.description || '';
        if (lmLink) lmLink.href = slides[index].dataset.link || '#';
    }

    function nextSlide() {
        currentIndex = (currentIndex + 1) % slides.length;
        showSlide(currentIndex);
    }

    function startSlideshow() {
        if (!slideInterval) slideInterval = setInterval(nextSlide, intervalTime);
    }

    function stopSlideshow() {
        if (resumeTimeout) {
            clearTimeout(resumeTimeout);
            resumeTimeout = null;
        }
        if (slideInterval) {
            clearInterval(slideInterval);
            slideInterval = null;
        }
    }

    showSlide(0);

    // Start rotation only after the loader overlay is fully removed from the DOM
    function startAfterLoader() {
        const loader = document.getElementById('loader');
        if (!loader) {
            startSlideshow();
            return;
        }
        const observer = new MutationObserver(() => {
            if (!document.getElementById('loader')) {
                observer.disconnect();
                startSlideshow();
            }
        });
        observer.observe(document.body, { childList: true });
    }
    startAfterLoader();

    if (panel) {
        // Hovering the panel pauses rotation and makes image fully visible
        panel.addEventListener('mouseenter', () => {
            document.body.classList.add('hovering-panel');
            stopSlideshow();
        });

        // Leaving the panel: advance to next slide after 1s, then resume normal cadence
        panel.addEventListener('mouseleave', () => {
            document.body.classList.remove('hovering-panel');
            resumeTimeout = setTimeout(() => {
                resumeTimeout = null;
                nextSlide();
                startSlideshow();
            }, 1000);
        });
    }

    if (panelBar) {
        panelBar.addEventListener('click', (e) => {
            e.stopPropagation();
            if (!isExpanded) {
                isExpanded = true;
                panel.classList.add('expanded');
            } else {
                isExpanded = false;
                panel.classList.remove('expanded');
                // Don't touch hovering-panel or slideshow here —
                // cursor is still over the panel, so mouseleave will handle cleanup
            }
        });
    }
});
