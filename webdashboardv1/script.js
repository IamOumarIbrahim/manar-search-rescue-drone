document.addEventListener('DOMContentLoaded', () => {
  // Channels configuration
  const channels = ['thermal', 'rgb', 'lowlight'];
  
  // Object to keep track of created Blob URLs for memory cleanup
  const videoUrls = {
    thermal: null,
    rgb: null,
    lowlight: null
  };

  // UI Elements
  const btnStart = document.getElementById('btn-start');
  const btnStop = document.getElementById('btn-stop');
  const systemStatus = document.getElementById('system-status');
  const statusText = systemStatus.querySelector('.status-text');

  // Helper function to handle video file selection
  channels.forEach((channel) => {
    const fileInput = document.getElementById(`input-${channel}`);
    const videoElem = document.getElementById(`video-${channel}`);
    const filenameTag = document.getElementById(`filename-${channel}`);
    const cardElem = document.getElementById(`card-${channel}`);

    fileInput.addEventListener('change', (e) => {
      const file = e.target.files[0];
      if (!file) return;

      // Revoke previous object URL if exists
      if (videoUrls[channel]) {
        URL.revokeObjectURL(videoUrls[channel]);
      }

      // Create new Object URL for uploaded video file
      const objectUrl = URL.createObjectURL(file);
      videoUrls[channel] = objectUrl;

      // Assign video source
      videoElem.src = objectUrl;
      videoElem.load();

      // Update UI labels and states
      filenameTag.textContent = file.name;
      filenameTag.classList.add('loaded');
      cardElem.classList.add('has-file');
    });
  });

  // START Button Handler: Play all uploaded video streams simultaneously
  btnStart.addEventListener('click', () => {
    let playedAny = false;

    channels.forEach((channel) => {
      const videoElem = document.getElementById(`video-${channel}`);
      const viewport = videoElem.closest('.video-viewport');
      const cardElem = document.getElementById(`card-${channel}`);

      if (videoUrls[channel] && videoElem.src) {
        videoElem.currentTime = 0;
        videoElem.play().then(() => {
          viewport.classList.add('active-stream');
          cardElem.classList.add('is-playing');
        }).catch((err) => {
          console.error(`Playback error on ${channel}:`, err);
        });
        playedAny = true;
      }
    });

    if (playedAny) {
      systemStatus.className = 'system-status-badge active';
      statusText.textContent = 'LIVE STREAMING';
    } else {
      alert('Please upload at least one video file before clicking START.');
    }
  });

  // STOP Button Handler: Stop video streams, revoke URLs, and remove uploaded video files
  btnStop.addEventListener('click', () => {
    channels.forEach((channel) => {
      const fileInput = document.getElementById(`input-${channel}`);
      const videoElem = document.getElementById(`video-${channel}`);
      const filenameTag = document.getElementById(`filename-${channel}`);
      const cardElem = document.getElementById(`card-${channel}`);
      const viewport = videoElem.closest('.video-viewport');

      // 1. Pause video
      videoElem.pause();
      videoElem.currentTime = 0;

      // 2. Clear video source
      videoElem.removeAttribute('src');
      videoElem.load();

      // 3. Revoke Blob Object URL from memory
      if (videoUrls[channel]) {
        URL.revokeObjectURL(videoUrls[channel]);
        videoUrls[channel] = null;
      }

      // 4. Clear file input selection
      fileInput.value = '';

      // 5. Reset UI tags and active states
      filenameTag.textContent = 'No file chosen';
      filenameTag.classList.remove('loaded');
      cardElem.classList.remove('has-file', 'is-playing');
      viewport.classList.remove('active-stream');
    });

    // Reset status badge
    systemStatus.className = 'system-status-badge stopped';
    statusText.textContent = 'STOPPED & CLEARED';

    setTimeout(() => {
      systemStatus.className = 'system-status-badge';
      statusText.textContent = 'SYSTEM STANDBY';
    }, 2500);
  });
});
