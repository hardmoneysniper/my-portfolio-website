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
    const intervalTime = 3000;

    function showSlide(index) {
        // Pause any video in slides becoming inactive
        slides.forEach(s => {
            s.classList.remove('active');
            const v = s.querySelector('video');
            if (v) v.pause();
        });

        const slide = slides[index];

        // lazy create on first visit
        if (slide.dataset.video) {
            let vid = slide.querySelector('video');
            if (!vid) {
                vid = document.createElement('video');
                vid.loop = true;
                vid.muted = true;
                vid.setAttribute('playsinline', '');
                // Override global video rule (max-width:80%; margin:100px auto 0)
                vid.style.cssText = [
                    'position:absolute', 'top:0', 'left:0',
                    'width:100%', 'height:100%',
                    'max-width:none', 'margin:0',
                    'object-fit:cover'
                ].join(';');
                const src = document.createElement('source');
                src.src = slide.dataset.video;
                src.type = 'video/quicktime';
                const src2 = document.createElement('source');
                src2.src = slide.dataset.video;
                src2.type = 'video/mp4';
                vid.appendChild(src);
                vid.appendChild(src2);
                slide.appendChild(vid);
            }
            vid.play();
        }

        if (!slide.style.backgroundImage && slide.dataset.bg) {
            slide.style.backgroundImage = `url('${slide.dataset.bg}')`;
        }

        slide.classList.add('active');
        if (lmTitle) lmTitle.textContent = slide.dataset.title || '';
        if (lmInfo) lmInfo.textContent = slide.dataset.info || '';
        if (lmDescription) lmDescription.textContent = slide.dataset.description || '';
        if (lmLink) lmLink.href = slide.dataset.link || '#';

        // Mobile: retrigger pop-out hint on each new slide
        if (panel && !isExpanded && window.matchMedia('(max-width: 499px)').matches) {
            panel.classList.remove('panel-hint');
            void panel.offsetWidth; // force reflow to restart animation
            panel.classList.add('panel-hint');
        }
    }

    function nextSlide() {
        currentIndex = (currentIndex + 1) % slides.length;
        showSlide(currentIndex);
    }

    function prevSlide() {
        currentIndex = (currentIndex - 1 + slides.length) % slides.length;
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
        panel.addEventListener('mouseenter', () => {
            document.body.classList.add('hovering-panel');
            stopSlideshow();
        });

        panel.addEventListener('mouseleave', () => {
            document.body.classList.remove('hovering-panel');
            resumeTimeout = setTimeout(() => {
                resumeTimeout = null;
                nextSlide();
                startSlideshow();
            }, 1000);
        });
    }

    // ── Mobile touch: swipe left = go to project, swipe right = prev slide ──
    const slideshowEl = document.querySelector('.slideshow-container');
    if (slideshowEl) {
        let touchX = 0, touchY = 0, touchT = 0;
        slideshowEl.addEventListener('touchstart', e => {
            touchX = e.touches[0].clientX;
            touchY = e.touches[0].clientY;
            touchT = Date.now();
        }, { passive: true });
        slideshowEl.addEventListener('touchend', e => {
            const dx = touchX - e.changedTouches[0].clientX;
            const dy = Math.abs(touchY - e.changedTouches[0].clientY);
            const dt = Date.now() - touchT;
            if (Math.abs(dx) < 40 || dy > Math.abs(dx)) return; // not a horizontal swipe
            stopSlideshow();
            if (dx > 0) {
                // Swipe left → visit current project
                const link = slides[currentIndex].dataset.link;
                if (link) { window.location.href = link; return; }
            } else {
                // Swipe right → previous slide
                prevSlide();
            }
            resumeTimeout = setTimeout(() => {
                resumeTimeout = null;
                startSlideshow();
            }, 1000);
        }, { passive: true });
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
                // On mobile, mouseleave never fires after Collapse, so restart manually
                if (window.matchMedia('(max-width: 499px)').matches) {
                    document.body.classList.remove('hovering-panel');
                    resumeTimeout = setTimeout(() => {
                        resumeTimeout = null;
                        nextSlide();
                        startSlideshow();
                    }, 2000);
                }
            }
        });
    }
});
