(function () {
    // Synchronously hide body before any content renders — prevents flash of page content.
    // Runs in <head> before the browser has painted anything.
    const preloadStyle = document.createElement('style');
    preloadStyle.id = 'loader-preload';
    preloadStyle.textContent = 'body{visibility:hidden}';
    document.head.appendChild(preloadStyle);

    // Safety: if init() never fires (JS error, etc.), reveal body after 5s
    const safetyTimer = setTimeout(() => {
        const s = document.getElementById('loader-preload');
        if (s) s.remove();
    }, 5000);

    const startTime = Date.now();
    const INTERVAL = 300;

    // Entry load (no same-origin referrer): 800–1200 ms. Internal navigation: 300–600 ms.
    function getMin() {
        try {
            if (document.referrer &&
                new URL(document.referrer).origin === window.location.origin) {
                return Math.random() * 300 + 300;   // 300–600 ms
            }
        } catch (_) {}
        return Math.random() * 400 + 800;           // 800–1200 ms
    }
    const MIN = getMin();

    const IMAGES = [
        { n: 1, type: 'portrait'  },
        { n: 2, type: 'landscape' },
        { n: 3, type: 'landscape' },
        { n: 4, type: 'portrait'  },
        { n: 5, type: 'portrait'  },
        { n: 6, type: 'landscape' },
        { n: 7, type: 'landscape' },
        { n: 8, type: 'landscape' },
    ];

    function getBase() {
        const link = document.querySelector('link[href*="style.css"]');
        return link && link.getAttribute('href').startsWith('../') ? '../' : '';
    }

    function init() {
        clearTimeout(safetyTimer);

        const base = getBase();

        const loader = document.createElement('div');
        loader.id = 'loader';

        const wrap = document.createElement('div');
        wrap.id = 'loader-images';

        IMAGES.forEach(({ n, type }, i) => {
            const img = document.createElement('img');
            img.src = `${base}loading/${n}.png`;
            img.className = `loader-img ${type}${i === 0 ? ' active' : ''}`;
            img.alt = '';
            wrap.appendChild(img);
        });

        loader.appendChild(wrap);
        document.body.insertBefore(loader, document.body.firstChild);

        // Loader is now in place — safe to reveal body (loader covers everything)
        const ps = document.getElementById('loader-preload');
        if (ps) ps.remove();

        const imgs = wrap.querySelectorAll('.loader-img');
        let idx = 0;

        const cycle = setInterval(() => {
            imgs[idx].classList.remove('active');
            idx = (idx + 1) % imgs.length;
            imgs[idx].classList.add('active');
        }, INTERVAL);

        const dismiss = () => {
            clearInterval(cycle);
            // Enable transition only for fade-out (no fade-in)
            loader.style.transition = 'opacity 0.35s ease';
            void loader.offsetHeight;
            loader.classList.add('fade-out');
            setTimeout(() => loader.remove(), 400);
        };

        window.addEventListener('load', () => {
            const elapsed = Date.now() - startTime;
            setTimeout(dismiss, Math.max(MIN - elapsed, 0));
        });
    }

    if (document.readyState === 'loading') {
        document.addEventListener('DOMContentLoaded', init);
    } else {
        init();
    }
}());
