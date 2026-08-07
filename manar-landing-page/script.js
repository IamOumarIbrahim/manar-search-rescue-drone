const translations = {
  en: {
    headerGithub: "GitHub",
    eyebrow: "MANAR / منار",
    heroTitle: "Supervised autonomy for search and rescue.",
    heroDescription:
      "MANAR is a search-and-rescue drone project built around multisensor perception, deterministic flight control, and human review for consequential actions.",
    viewGithub: "View on GitHub",
    tagAutonomy: "Supervised autonomy",
    tagSensors: "Multisensor",
    tagHuman: "Human in the loop",
    approachEyebrow: "Project approach",
    approachTitle: "Designed around accountable rescue assistance.",
    card1Title: "Multisensor perception",
    card1Text:
      "Combines complementary sensing modalities to improve situational awareness across varied conditions.",
    card2Title: "Supervised autonomy",
    card2Text:
      "Automates repeatable search tasks while keeping important operational decisions under human oversight.",
    card3Title: "Rescue-first design",
    card3Text:
      "Calm, technically credible, civilian system design focused on search, verification, and guidance.",
    componentsEyebrow: "Hardware Architecture",
    componentsTitle: "System Component Overview",
    thComponent: "Component",
    thRole: "Primary role",
    c1Name: "Thermal",
    c1Role: "Person/heat detection",
    c2Name: "RGB/day",
    c2Role: "Daytime detection/verification",
    c3Name: "Low-light/IR",
    c3Role: "Night visual confirmation",
    c4Name: "24 GHz FMCW",
    c4Role: "Presence, range, motion, breathing",
    c5Name: "Speaker + mic",
    c5Role: "Prompt, listen, direction finding",
    c6Name: "Passive RF",
    c6Role: "Detect/correlate device emissions",
    c7Name: "Amber beacon",
    c7Role: "360° visual alert",
    c8Name: "White strobe",
    c8Role: "Directional visual guidance",
    c9Name: "Downward spotlight",
    c9Role: "Close-range illumination",
    c10Name: "Heliograph mirrors",
    c10Role: "Passive daylight signaling",
    c11Name: "Smoke marker",
    c11Role: "Location/wind marking",
    milestonesTitle: "Project Milestones",
    thMilestone: "Milestone",
    thStatus: "Status",
    statusCompleted: "Completed ✓",
    statusPlanned: "Planned ⋯",
    m1: "Initial Ideation",
    m2: "GitHub Repository creation",
    m3: "Initial Branding",
    m4: "Project scope hardening",
    m5: "Project limitations and constraints",
    m6: "Basic web dashboard — v1.0",
    m7: "Landing Page",
    m8: "Initial deterministic control system design",
    m9: "To be determined...",
    m10: "LaTeX Report, Presentation and Full documentation",
    m11: "Final Publishing & LinkedIn Post",
    m12: "Repository Maintenance",
    ctaEyebrow: "Project repository",
    ctaTitle: "Follow MANAR on GitHub.",
    ctaText: "See the current project overview, milestones, and public portfolio materials.",
    openRepository: "Open repository",
    footerText: "© 2026 Oumar Ibrahim. All rights reserved."
  },
  ar: {
    headerGithub: "GitHub",
    eyebrow: "منار / MANAR",
    heroTitle: "استقلالية تحت الإشراف للبحث والإنقاذ.",
    heroDescription:
      "منار هو مشروع طائرة مسيّرة للبحث والإنقاذ يعتمد على الاستشعار متعدد المصادر، والتحكم الحتمي في الطيران، والمراجعة البشرية للقرارات ذات الأثر المهم.",
    viewGithub: "عرض المشروع على GitHub",
    tagAutonomy: "استقلالية تحت الإشراف",
    tagSensors: "متعدد الحساسات",
    tagHuman: "الإنسان ضمن الحلقة",
    approachEyebrow: "منهج المشروع",
    approachTitle: "مصمم لمساندة عمليات الإنقاذ بمسؤولية ووضوح.",
    card1Title: "إدراك متعدد الحساسات",
    card1Text:
      "يجمع بين وسائل استشعار متكاملة لتحسين الوعي بالموقف في ظروف تشغيل متنوعة.",
    card2Title: "استقلالية تحت الإشراف",
    card2Text:
      "يؤتمت مهام البحث المتكررة مع إبقاء القرارات التشغيلية المهمة تحت إشراف الإنسان.",
    card3Title: "تصميم موجه للإنقاذ",
    card3Text:
      "نهج مدني هادئ وموثوق تقنياً يركز على البحث والتحقق والتوجيه.",
    componentsEyebrow: "البنية العتادية",
    componentsTitle: "نظرة عامة على مكونات النظام",
    thComponent: "المكون",
    thRole: "الدور الرئيسي",
    c1Name: "الكاميرا الحرارية (Thermal)",
    c1Role: "كشف الأشخاص والبصمة الحرارية",
    c2Name: "الكاميرا الضوئية النهارية (RGB/day)",
    c2Role: "الكشف والتحقق في النهار",
    c3Name: "كاميرا الإضاءة المنخفضة (Low-light/IR)",
    c3Role: "التأكيد البصري الليلي",
    c4Name: "رادار 24 جيجاهرتز FMCW",
    c4Role: "كشف الوجود والمدى والحركة والنفس",
    c5Name: "مكبر صوت وميكروفون (Speaker + mic)",
    c5Role: "التنبيه، الاستماع، وتحديد الاتجاه",
    c6Name: "مستقبل لاسلكي سلبي (Passive RF)",
    c6Role: "كشف ومطابقة انبعاثات الأجهزة",
    c7Name: "منارة ضوئية كهرمانية (Amber beacon)",
    c7Role: "تنبيه بصري 360 درجة",
    c8Name: "ومّاض أبيض (White strobe)",
    c8Role: "توجيه بصري موجه",
    c9Name: "كشاف إضاءة سفلي (Downward spotlight)",
    c9Role: "إضاءة السطح والمداهمة القريبة",
    c10Name: "مرايا هليوجراف (Heliograph mirrors)",
    c10Role: "إشارات نهارية سلبية بالضوء",
    c11Name: "علامة دخانية (Smoke marker)",
    c11Role: "تحديد الموقع واتجاه الرياح",
    milestonesTitle: "معالم المشروع",
    thMilestone: "المرحلة",
    thStatus: "الحالة",
    statusCompleted: "مكتمل ✓",
    statusPlanned: "مخطط ⋯",
    m1: "الفكرة الأولية",
    m2: "إنشاء مستودع GitHub",
    m3: "الهوية البصرية الأولية",
    m4: "تحديد نطاق المشروع",
    m5: "تحديد القيود والشروط",
    m6: "لوحة التحكم الإلكتورنية — v1.0",
    m7: "صفحة هبوط المشروع (Landing Page)",
    m8: "تصميم نظام التحكم الحتمي الأول",
    m9: "سيتم تحديده لاحقاً...",
    m10: "تقرير LaTeX والعرض والتوثيق الكامل",
    m11: "النشر النهائي ومنشور LinkedIn",
    m12: "صيانة وتحديث المستودع",
    ctaEyebrow: "مستودع المشروع",
    ctaTitle: "تابع مشروع منار على GitHub.",
    ctaText: "اطّلع على نظرة المشروع الحالية والمراحل والمواد العامة الخاصة بالمشروع.",
    openRepository: "فتح المستودع",
    footerText: "حقوق الطبع والنشر © 2026 عمر إبراهيم. جميع الحقوق محفوظة."
  }
};

const languageButtons = document.querySelectorAll("[data-language]");
const translatableElements = document.querySelectorAll("[data-i18n]");

function applyLanguage(language) {
  const dictionary = translations[language] || translations.en;
  const isArabic = language === "ar";

  document.documentElement.lang = language;
  document.documentElement.dir = isArabic ? "rtl" : "ltr";

  translatableElements.forEach((element) => {
    const key = element.dataset.i18n;
    if (dictionary[key]) element.textContent = dictionary[key];
  });

  languageButtons.forEach((button) => {
    const active = button.dataset.language === language;
    button.classList.toggle("is-active", active);
    button.setAttribute("aria-pressed", String(active));
  });

  localStorage.setItem("manar-language", language);
}

languageButtons.forEach((button) => {
  button.addEventListener("click", () => applyLanguage(button.dataset.language));
});

const savedLanguage = localStorage.getItem("manar-language");
const browserLanguage = navigator.language?.toLowerCase().startsWith("ar") ? "ar" : "en";
applyLanguage(savedLanguage || browserLanguage);

// Ambient Audio Controller (Video Container Audio)
const bgAudio = document.getElementById("bgAudio");
const audioMuteBtn = document.getElementById("audioMuteBtn");
const audioIcon = document.getElementById("audioIcon");
const audioVolumeSlider = document.getElementById("audioVolumeSlider");

let isMuted = localStorage.getItem("manar-audio-muted") === "true";
let savedVolume = parseFloat(localStorage.getItem("manar-audio-volume") || "0.3");

if (bgAudio && audioMuteBtn && audioVolumeSlider) {
  bgAudio.volume = savedVolume;
  audioVolumeSlider.value = savedVolume;

  function updateAudioUI() {
    const muted = isMuted || bgAudio.muted || bgAudio.volume === 0;
    audioIcon.textContent = muted ? "🔇" : "🔊";
    audioMuteBtn.setAttribute("aria-label", muted ? "Unmute background sound" : "Mute background sound");
  }

  function enableAudioOnInteraction() {
    if (isMuted) return;
    bgAudio.muted = false;
    bgAudio.volume = parseFloat(audioVolumeSlider.value) || 0.3;
    bgAudio.play().then(() => {
      updateAudioUI();
    }).catch(() => {});
  }

  ["scroll", "click", "keydown", "touchstart", "wheel"].forEach((event) => {
    window.addEventListener(event, enableAudioOnInteraction, { passive: true });
  });

  audioMuteBtn.addEventListener("click", (e) => {
    e.stopPropagation();
    isMuted = !isMuted;
    localStorage.setItem("manar-audio-muted", isMuted);
    if (isMuted) {
      bgAudio.pause();
    } else {
      bgAudio.muted = false;
      bgAudio.play().catch(() => {});
    }
    updateAudioUI();
  });

  audioVolumeSlider.addEventListener("input", (e) => {
    e.stopPropagation();
    const val = parseFloat(e.target.value);
    bgAudio.volume = val;
    localStorage.setItem("manar-audio-volume", val);
    if (val > 0) {
      isMuted = false;
      localStorage.setItem("manar-audio-muted", "false");
      bgAudio.muted = false;
      bgAudio.play().catch(() => {});
    } else {
      isMuted = true;
      localStorage.setItem("manar-audio-muted", "true");
      bgAudio.pause();
    }
    updateAudioUI();
  });

  updateAudioUI();
}
