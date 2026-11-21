// web_page.h - trang web điều khiển (HTML + CSS + JS trong một chuỗi, nằm trong flash, KHÔNG cần Internet).
// Tạo từ index.src.html; sửa giao diện thì sửa chuỗi bên dưới (đừng để xuất hiện ký tự )HTML" trong nội dung).
// Font chữ số/tiêu đề: Nunito ExtraBold (SIL Open Font License 1.1), đã cắt còn các ký tự dùng trong trang.
#pragma once
#include <Arduino.h>

static const char WEB_PAGE[] PROGMEM = R"HTML(<!doctype html>
<html lang="vi">
<head>
<meta charset="utf-8">
<meta name="viewport" content="width=device-width, initial-scale=1, viewport-fit=cover">
<meta name="theme-color" content="#86bfe6">
<title>Tưới cây thông minh</title>
<style>
@font-face{font-family:"TC";font-weight:800;font-style:normal;font-display:swap;src:url(data:font/woff2;base64,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) format("woff2")}  /* Nunito ExtraBold, SIL OFL 1.1 */
/* ===== Màu: lấy từ chính màn hình TFT (trời, cỏ, đất, xanh bạc hà) ===== */
:root{
  --sky-1:#86bfe6; --sky-2:#b9dcf0; --sky-3:#e3eff2;
  --ink:#16402e;                         /* chữ đậm trên nền trời */
  --grass-1:#6bbd55; --grass-2:#3c8a3a;
  --soil-1:#46291b; --soil-2:#2b1a12; --soil-3:#22140e;
  --sand:#f4e0c4; --clay:#cdb094;        /* chữ chính / chữ phụ trên đất */
  --leaf:#52c37b; --leaf-dk:#0c3a21;     /* nút đang chọn */
  --heat:#c6500e; --water:#1668a2;       /* nhiệt độ / độ ẩm không khí (trên nền trời) */
  --z0:#f56753; --z1:#fea846; --z2:#54ce80; --z3:#48afed; --z4:#8c7e72;   /* 4 vùng độ ẩm đất + lỗi */
  --display:"TC","Nunito","Segoe UI",system-ui,-apple-system,"Roboto",sans-serif;
  --text:system-ui,-apple-system,"Segoe UI","Roboto","Noto Sans",sans-serif;
  --px:76%;                              /* vị trí ngang của cây + vòi nước */
}
@media(min-width:780px){:root{--px:50%}}
*{box-sizing:border-box}
html{background:var(--soil-3);-webkit-text-size-adjust:100%}
body{margin:0;min-height:100vh;color:var(--sand);font:16px/1.5 var(--text);background:linear-gradient(var(--soil-1),var(--soil-2) 55%,var(--soil-3))}
h1,h2,.big,.air b,button{font-family:var(--display)}
h1,h2,p{margin:0}
.wrap{max-width:1040px;margin:0 auto;padding:0 max(20px,env(safe-area-inset-right)) 0 max(20px,env(safe-area-inset-left))}
:focus-visible{outline:3px solid #fff;outline-offset:3px;border-radius:12px}

/* ===== Bầu trời: tên hệ thống + nhiệt độ + độ ẩm không khí ===== */
.sky{position:relative;background:linear-gradient(var(--sky-1),var(--sky-2) 58%,var(--sky-3));color:var(--ink);padding-top:max(18px,env(safe-area-inset-top))}
.sunclip{position:absolute;left:0;right:0;top:0;bottom:0;overflow:hidden;pointer-events:none}
.sun{position:absolute;top:-70px;right:-60px;width:210px;height:210px;border-radius:50%;background:radial-gradient(circle,#fff6c4 0 28%,rgba(255,236,150,.55) 29% 46%,rgba(255,236,150,0) 70%)}
@media(min-width:560px){.sun{top:34px;right:5%}}
.top{position:relative;display:flex;flex-wrap:wrap;gap:12px 16px;align-items:center;justify-content:space-between}
.brand{display:flex;gap:12px;align-items:center}
.logo{width:44px;height:44px;flex:none}
h1{font-size:23px;line-height:1.1;font-weight:800;letter-spacing:-.01em}
.brand p{font-size:14px;opacity:.82}
.link{display:inline-flex;align-items:center;gap:8px;padding:6px 12px 6px 10px;border-radius:999px;background:rgba(255,255,255,.62);font-size:14px;font-weight:600}
.link .dot{width:9px;height:9px;border-radius:50%;background:#e5534b}
.link[data-on="1"] .dot{background:#25a55a}
.bars{display:inline-flex;align-items:flex-end;gap:2px;height:14px}
.bars i{width:4px;border-radius:1px;background:currentColor;opacity:.22}
.bars i:nth-child(1){height:5px}.bars i:nth-child(2){height:9px}.bars i:nth-child(3){height:14px}
.bars[data-n="1"] i:nth-child(-n+1),.bars[data-n="2"] i:nth-child(-n+2),.bars[data-n="3"] i{opacity:1}
.air{position:relative;display:flex;gap:32px;flex-wrap:wrap;margin-top:18px}
.read{display:flex;flex-direction:column}
.read>span{font-size:14px;font-weight:600;opacity:.85}
.read b{font-size:46px;line-height:1.05;font-weight:800;letter-spacing:-.01em}
.read small{font-size:.5em;font-weight:800;margin-left:2px}
.read.heat b{color:var(--heat)}
.read.water b{color:var(--water)}
.horizon{position:relative;height:74px;margin-top:16px;z-index:2}
.grass{position:absolute;left:0;bottom:0;width:100%;height:46px;display:block}
.sprout{position:absolute;bottom:0;left:calc(var(--px) - 38px);width:76px}
.pipe{position:absolute;bottom:29px;left:calc(var(--px) - 82px);width:30px}
.drop{position:absolute;left:calc(var(--px) - 61px);top:26px;width:8px;height:11px;border-radius:50% 50% 50% 50%/62% 62% 38% 38%;background:#6fc3f4;opacity:0}
body[data-pump="1"] .drop{animation:fall 1.3s cubic-bezier(.5,0,.9,.6) infinite}
body[data-pump="1"] .drop:nth-of-type(2){animation-delay:.43s}
body[data-pump="1"] .drop:nth-of-type(3){animation-delay:.86s}
@keyframes fall{0%{transform:translateY(0);opacity:0}15%{opacity:1}100%{transform:translateY(38px);opacity:0}}

/* ===== Lòng đất: độ ẩm đất (to nhất) + điều khiển ===== */
.soil{position:relative;padding:22px 0 40px}
.rootsclip{position:absolute;left:0;right:0;top:0;height:150px;overflow:hidden;pointer-events:none}
.roots{position:absolute;top:0;left:calc(var(--px) - 120px);width:240px;height:150px;opacity:.15}
.grid{position:relative;display:grid;gap:34px}
@media(min-width:780px){.grid{grid-template-columns:1.12fr 1fr;gap:64px}.air{gap:56px}}
h2{font-size:20px;line-height:1.2;font-weight:800;display:flex;align-items:center;gap:10px;flex-wrap:wrap}
.badge{font:700 14px/1 var(--display);padding:6px 11px;border-radius:999px;color:var(--c,var(--clay));background:color-mix(in srgb,var(--c,var(--clay)) 17%,transparent);box-shadow:inset 0 0 0 1px color-mix(in srgb,var(--c,var(--clay)) 45%,transparent)}
[data-z="0"]{--c:var(--z0)}[data-z="1"]{--c:var(--z1)}[data-z="2"]{--c:var(--z2)}[data-z="3"]{--c:var(--z3)}[data-z="4"]{--c:#bcac9e}
.big{margin-top:6px;font-size:clamp(104px,32vw,176px);line-height:.92;font-weight:800;letter-spacing:-.03em;font-variant-numeric:tabular-nums;color:var(--c);background:linear-gradient(var(--c1),var(--c2));-webkit-background-clip:text;background-clip:text;-webkit-text-fill-color:transparent}
.big small{font-size:.42em;letter-spacing:0;margin-left:.04em}
[data-z="0"] .big,.big[data-z="0"]{--c1:#ffa995;--c2:#f56753}
.big[data-z="1"]{--c1:#ffcf83;--c2:#fea846}
.big[data-z="2"]{--c1:#9dedb4;--c2:#54ce80}
.big[data-z="3"]{--c1:#99d8fb;--c2:#48afed}
.big[data-z="4"]{--c1:#bcac9e;--c2:#8c7e72}
.zones{position:relative;display:grid;grid-template-columns:repeat(4,1fr);gap:6px;margin-top:22px}
.zones i{height:10px;border-radius:5px}
.zones i:nth-child(1){background:var(--z0)}.zones i:nth-child(2){background:var(--z1)}.zones i:nth-child(3){background:var(--z2)}.zones i:nth-child(4){background:var(--z3)}
.knob{position:absolute;left:0;top:-6px;width:22px;height:22px;margin-left:-11px;border-radius:50%;background:#fff7e8;box-shadow:0 0 0 4px rgba(34,20,14,.55),0 2px 8px rgba(0,0,0,.45);transition:transform .7s cubic-bezier(.2,.8,.2,1);display:none}
.knob::after{content:"";position:absolute;inset:6px;border-radius:50%;background:var(--c,var(--z2))}
.zl{list-style:none;margin:10px 0 0;padding:0;display:grid;grid-template-columns:repeat(4,1fr);gap:6px;text-align:center}
.zl b{display:block;font:800 14px/1.25 var(--display);color:var(--clay)}
.zl small{display:block;white-space:nowrap;font-size:12.5px;color:var(--clay);opacity:.8}
.zl li[data-on="1"] b{color:var(--c)}
.note{margin-top:12px;font-size:14px;color:var(--clay);max-width:44ch;text-wrap:pretty}
.note:empty{display:none}
.note.warn{color:#ffd9a0}

/* chế độ: công tắc hai nửa giống màn TFT */
.ctl{display:flex;flex-direction:column;gap:14px}
.ctl h2:not(:first-child){margin-top:20px}
.seg{display:grid;grid-template-columns:1fr 1fr;gap:4px;padding:4px;border-radius:999px;background:rgba(0,0,0,.34);box-shadow:inset 0 0 0 1px rgba(255,255,255,.07)}
.seg button{min-height:52px;border:0;border-radius:999px;background:transparent;color:var(--sand);font-size:18px;font-weight:800;cursor:pointer;transition:background .2s,color .2s}
.seg button[aria-pressed="true"]{background:linear-gradient(#66d28e,#3fb56c);color:var(--leaf-dk);box-shadow:0 2px 10px rgba(0,0,0,.35)}
.seg button:not([aria-pressed="true"]):hover{background:rgba(255,255,255,.07)}

/* bơm: nút lớn, bấm bằng ngón cái */
.pump{display:flex;align-items:center;justify-content:center;gap:12px;width:100%;min-height:64px;border:0;border-radius:32px;font-size:21px;font-weight:800;cursor:pointer;color:var(--leaf-dk);background:linear-gradient(#66d28e,#3fb56c);box-shadow:0 3px 14px rgba(0,0,0,.4);transition:filter .15s,transform .1s}
.pump:active:not(:disabled){transform:scale(.985)}
.pump svg{width:24px;height:24px;flex:none}
body[data-pump="1"] .pump{background:linear-gradient(#8fd3fb,#48afed);color:#08304d}
.pump:disabled{cursor:not-allowed;color:var(--clay);background:rgba(0,0,0,.32);box-shadow:inset 0 0 0 1px rgba(255,255,255,.09)}
body[data-pump="1"] .pump:disabled{color:#08304d;background:linear-gradient(#8fd3fb,#48afed);filter:saturate(.3) brightness(.7);box-shadow:none}
.seg button:disabled{cursor:not-allowed;opacity:.55}
body[data-busy="1"] .pump,body[data-busy="1"] .seg{filter:saturate(.6)}

/* chân trang: pin + giờ cập nhật */
.foot{margin-top:38px;padding-top:18px;border-top:1px solid rgba(244,224,196,.14);display:flex;flex-wrap:wrap;gap:8px 28px;font-size:14px;color:var(--clay);align-items:center}
.batt{display:inline-flex;align-items:center;gap:8px;font-weight:600;color:var(--sand)}
.cell{position:relative;width:26px;height:13px;border-radius:3px;box-shadow:inset 0 0 0 2px var(--clay)}
.cell::after{content:"";position:absolute;right:-5px;top:3px;width:3px;height:7px;border-radius:0 2px 2px 0;background:var(--clay)}
.cell i{position:absolute;left:2px;top:2px;bottom:2px;width:calc((100% - 4px) * var(--p,0));border-radius:1px;background:#98ecaa}
.cell[data-lv="1"] i{background:#ffd680}.cell[data-lv="2"] i{background:#ff9680}
.chg{color:#98ecaa;font-weight:700}

/* thông báo lỗi và mất kết nối */
.off{display:none;position:sticky;top:0;z-index:5;padding:10px 20px;text-align:center;background:#7a1f1a;color:#ffe9e5;font-size:15px;font-weight:600}
body[data-off="1"] .off{display:block}
.toast{position:fixed;left:50%;bottom:max(18px,env(safe-area-inset-bottom));transform:translate(-50%,24px);max-width:min(92vw,460px);padding:12px 18px;border-radius:14px;background:#fff1d9;color:#4a2a10;font-size:15px;font-weight:600;box-shadow:0 8px 28px rgba(0,0,0,.45);opacity:0;pointer-events:none;transition:opacity .2s,transform .2s;z-index:9}
.toast[data-show="1"]{opacity:1;transform:translate(-50%,0)}
/* ===== Đổi Wi-Fi: nút ở đầu trang + khung trượt lên ===== */
.chips{display:flex;flex-wrap:wrap;gap:8px;align-items:center}
.wbtn{display:inline-flex;align-items:center;gap:8px;min-height:36px;padding:6px 14px 6px 11px;border:0;border-radius:999px;background:var(--ink);color:#e9f6ec;font-size:14px;font-weight:800;cursor:pointer}
.wbtn svg{width:18px;height:18px;flex:none}
.wbtn:hover{filter:brightness(1.18)}
html[data-lock="1"]{overflow:hidden}
.ov{position:fixed;inset:0;z-index:20;display:none;align-items:flex-end;justify-content:center;background:rgba(10,6,4,.64)}
.ov[data-open="1"]{display:flex}
.sheet{width:100%;max-width:520px;max-height:92vh;max-height:92dvh;overflow:auto;overscroll-behavior:contain;background:linear-gradient(var(--soil-1),var(--soil-2));color:var(--sand);border-radius:22px 22px 0 0;padding:16px max(20px,env(safe-area-inset-right)) max(24px,env(safe-area-inset-bottom)) max(20px,env(safe-area-inset-left));box-shadow:0 -10px 40px rgba(0,0,0,.5)}
@media(min-width:560px){.ov{align-items:center}.sheet{border-radius:22px;padding-bottom:24px}}
.sh{display:flex;align-items:center;justify-content:space-between;gap:12px}
.sh h2{font-size:21px}
.x{flex:none;width:44px;height:44px;border:0;border-radius:50%;background:rgba(255,255,255,.09);color:var(--sand);font-size:26px;line-height:1;cursor:pointer}
.wst{display:flex;gap:12px;align-items:flex-start;margin-top:12px;padding:12px 14px;border-radius:14px;background:rgba(0,0,0,.3);box-shadow:inset 0 0 0 1px rgba(255,255,255,.07)}
.wst .dot{flex:none;width:10px;height:10px;margin-top:7px;border-radius:50%;background:var(--z4)}
.wst[data-k="1"] .dot{background:var(--z2)}
.wst[data-k="2"] .dot{background:var(--z1)}
.wst b{display:block;font:800 16px/1.35 var(--display);overflow-wrap:anywhere}
.wst small{display:block;margin-top:2px;font-size:14px;line-height:1.4;color:var(--clay);overflow-wrap:anywhere}
.wrow{display:flex;align-items:center;justify-content:space-between;gap:12px;margin:22px 0 10px}
.wrow h3{margin:0;font:800 17px/1.2 var(--display)}
.ghost{min-height:44px;padding:0 18px;border:0;border-radius:22px;background:rgba(255,255,255,.1);color:var(--sand);font-size:15px;font-weight:800;cursor:pointer;box-shadow:inset 0 0 0 1px rgba(255,255,255,.14)}
.ghost:disabled{opacity:.6;cursor:progress}
.nets{display:flex;flex-direction:column;gap:6px;max-height:232px;overflow:auto;overscroll-behavior:contain}
.nets:empty{display:none}
.nets p{margin:0;font-size:14px;line-height:1.45;color:var(--clay)}
.net{display:flex;align-items:center;gap:10px;width:100%;min-height:48px;padding:6px 14px;border:0;border-radius:12px;background:rgba(0,0,0,.28);color:var(--sand);text-align:left;font:600 16px/1.3 var(--text);cursor:pointer}
.net:hover{background:rgba(255,255,255,.07)}
.net .nm{flex:1;min-width:0;overflow:hidden;text-overflow:ellipsis;white-space:nowrap}
.net .tag{flex:none;font-size:12.5px;font-weight:700;color:#98ecaa}
.net .lk{flex:none;display:inline-flex;opacity:.85}
.net .bars{flex:none;color:var(--sand)}
.net[aria-pressed="true"]{background:rgba(82,195,123,.18);box-shadow:inset 0 0 0 2px var(--leaf)}
.wf label{display:block;margin:18px 0 6px;font:800 15px/1.2 var(--display)}
.inp{display:block;width:100%;min-height:50px;padding:10px 14px;border:0;border-radius:12px;background:rgba(0,0,0,.36);color:var(--sand);font:17px/1.3 var(--text);box-shadow:inset 0 0 0 1px rgba(255,255,255,.2)}
.pw{position:relative}
.pw .inp{padding-right:84px}
.eye{position:absolute;right:6px;top:6px;height:38px;min-width:64px;padding:0 12px;border:0;border-radius:10px;background:rgba(255,255,255,.1);color:var(--sand);font-size:14px;font-weight:800;cursor:pointer}
.hint{margin-top:8px;font-size:14px;line-height:1.45;color:var(--clay)}
.go{display:flex;align-items:center;justify-content:center;width:100%;min-height:58px;margin-top:20px;border:0;border-radius:29px;font-size:19px;font-weight:800;cursor:pointer;color:var(--leaf-dk);background:linear-gradient(#66d28e,#3fb56c);box-shadow:0 3px 14px rgba(0,0,0,.4)}
.unl{margin-top:14px;color:#fff;background:linear-gradient(#f58a73,#e0523f)}
#unlockBtn[hidden]{display:none}
.go:disabled{cursor:progress;color:var(--clay);background:rgba(0,0,0,.32);box-shadow:inset 0 0 0 1px rgba(255,255,255,.09)}
.wmsg{margin-top:14px;padding:12px 14px;border-radius:12px;font-size:15px;line-height:1.5;background:rgba(0,0,0,.3);color:var(--sand);overflow-wrap:anywhere}
.wmsg:empty{display:none}
.wmsg[data-k="ok"]{background:rgba(82,195,123,.18);box-shadow:inset 0 0 0 1px rgba(82,195,123,.55)}
.wmsg[data-k="err"]{background:rgba(245,103,83,.16);box-shadow:inset 0 0 0 1px rgba(245,103,83,.55);color:#ffddd6}
.sr{position:absolute;width:1px;height:1px;overflow:hidden;clip:rect(0 0 0 0);white-space:nowrap}
@media(prefers-reduced-motion:reduce){*{transition:none!important;animation:none!important}body[data-pump="1"] .drop:nth-of-type(1){opacity:1;transform:translateY(24px)}}
</style>
</head>
<body data-pump="0" data-off="0" data-busy="0">
<div class="off" role="alert">Mất kết nối với thiết bị. Đang thử lại.</div>

<header class="sky">
  <div class="sunclip" aria-hidden="true"><div class="sun"></div></div>
  <div class="wrap">
    <div class="top">
      <div class="brand">
        <svg class="logo" viewBox="0 0 44 44" aria-hidden="true"><circle cx="22" cy="22" r="22" fill="#3fb56c"/><path d="M22 33V21" stroke="#fff" stroke-width="3" stroke-linecap="round"/><path d="M22 22c0-6-4-9.5-10-9.5 0 6 4 9.5 10 9.5z" fill="#fff"/><path d="M22 24c0-5 3.4-8 8.5-8 0 5-3.4 8-8.5 8z" fill="#fff" opacity=".85"/></svg>
        <div><h1>Tưới cây thông minh</h1><p>Giám sát và điều khiển tưới cây</p></div>
      </div>
      <div class="chips">
        <div class="link" id="link" data-on="0"><span class="dot"></span><span id="linkTxt">Đang kết nối</span><span class="bars" id="bars" data-n="0" aria-hidden="true"><i></i><i></i><i></i></span></div>
        <button type="button" class="wbtn" id="wBtn" aria-haspopup="dialog"><svg viewBox="0 0 24 24" aria-hidden="true"><path d="M2.5 9a15 15 0 0 1 19 0M5.5 12.6a10.5 10.5 0 0 1 13 0M8.7 16.2a6 6 0 0 1 6.6 0" fill="none" stroke="currentColor" stroke-width="2.2" stroke-linecap="round"/><circle cx="12" cy="19.6" r="1.7" fill="currentColor"/></svg>Đổi Wi-Fi</button>
      </div>
    </div>
    <div class="air">
      <div class="read heat"><span>Nhiệt độ</span><b><span id="temp">--</span><small>°C</small></b></div>
      <div class="read water"><span>Độ ẩm không khí</span><b><span id="hum">--</span><small>%</small></b></div>
    </div>
  </div>
  <div class="horizon" aria-hidden="true">
    <svg class="grass" width="100%" height="46">
      <defs>
        <pattern id="b1" width="26" height="46" patternUnits="userSpaceOnUse"><path d="M1 46 4 14 8 46Zm10 0 4-30 4 30Z" fill="#7cc866"/></pattern>
        <pattern id="b2" x="9" width="26" height="46" patternUnits="userSpaceOnUse"><path d="M1 46 5 20 9 46Zm11 0 3-22 5 22Z" fill="#4e9c43"/></pattern>
      </defs>
      <rect y="20" width="100%" height="26" fill="#3c8a3a"/>
      <rect width="100%" height="46" fill="url(#b1)"/>
      <rect width="100%" height="46" fill="url(#b2)"/>
      <rect y="34" width="100%" height="12" fill="#357d35"/>
    </svg>
    <svg class="sprout" viewBox="0 0 76 82"><path d="M38 82V26" stroke="#3c8a3a" stroke-width="4" stroke-linecap="round" fill="none"/><path d="M38 30C38 14 26 6 8 8c0 16 12 24 30 22z" fill="#5db04a"/><path d="M38 27C38 12 50 4 70 6c0 15-12 23-32 21z" fill="#86d36f"/></svg>
    <svg class="pipe" viewBox="0 0 30 50"><path d="M5 50V18a10 10 0 0 1 20 0v8" stroke="#8d9faa" stroke-width="4.5" fill="none" stroke-linecap="round"/><rect x="21" y="25" width="8" height="6" rx="2" fill="#6c7e89"/></svg>
    <i class="drop"></i><i class="drop"></i><i class="drop"></i>
  </div>
</header>

<main class="soil">
  <div class="rootsclip" aria-hidden="true"><svg class="roots" viewBox="0 0 240 150" aria-hidden="true" fill="none" stroke="#f4e0c4" stroke-width="3" stroke-linecap="round"><path d="M120 0v46M120 46C110 70 88 84 66 108M120 46c4 26 22 44 40 70M120 46c-2 30-4 60 4 96M66 108c-8 8-14 18-18 32M160 116c6 10 14 18 28 26"/></svg></div>
  <div class="wrap grid">
    <section class="moist" aria-labelledby="hSoil">
      <h2 id="hSoil">Độ ẩm đất <span class="badge" id="zone" data-z="2">Lý tưởng</span></h2>
      <p class="big" id="big" data-z="2"><span id="soil">--</span><small>%</small></p>
      <div class="zones" id="zones"><i></i><i></i><i></i><i></i><span class="knob" id="knob"></span></div>
      <ul class="zl" id="zl">
        <li data-z="0"><b>Rất khô</b><small>x&lt;20%</small></li>
        <li data-z="1"><b>Khô</b><small>20≤x&lt;30%</small></li>
        <li data-z="2"><b>Lý tưởng</b><small>30≤x≤50%</small></li>
        <li data-z="3"><b>Ướt</b><small>x&gt;50%</small></li>
      </ul>
      <p class="note warn" id="soilNote"></p>
    </section>

    <section class="ctl" aria-label="Điều khiển">
      <h2>Chế độ vận hành</h2>
      <div class="seg" role="group" aria-label="Chế độ vận hành">
        <button type="button" id="mAuto" aria-pressed="false">Tự động</button>
        <button type="button" id="mMan" aria-pressed="false">Thủ công</button>
      </div>
      <p class="note" id="modeNote" aria-live="polite"></p>

      <h2>Bơm tưới <span class="badge" id="pumpState" data-z="4">Đã tắt</span></h2>
      <button type="button" class="pump" id="pumpBtn" disabled>
        <svg viewBox="0 0 24 24" aria-hidden="true"><path d="M12 2.5C9 7.2 5.5 10.6 5.5 14.6a6.5 6.5 0 0 0 13 0C18.5 10.6 15 7.2 12 2.5z" fill="currentColor"/></svg>
        <span id="pumpTxt">Bật bơm</span>
      </button>
      <p class="note" id="pumpNote"></p>
      <button type="button" class="go unl" id="unlockBtn" hidden>Mở khóa bơm</button>
    </section>
  </div>

  <div class="wrap">
    <div class="foot">
      <span class="batt"><span class="cell" id="cell" data-lv="0"><i></i></span><span id="batTxt">Pin --</span><span class="chg" id="chgTxt"></span></span>
      <span id="stamp">Chưa có dữ liệu</span>
    </div>
  </div>
</main>

<div class="toast" id="toast" role="status" aria-live="polite"></div>

<div class="ov" id="ov" data-open="0">
  <div class="sheet" id="sheet" role="dialog" aria-modal="true" aria-labelledby="wTitle">
    <div class="sh">
      <h2 id="wTitle">Đổi Wi-Fi cho thiết bị</h2>
      <button type="button" class="x" id="wClose" aria-label="Đóng">×</button>
    </div>
    <div class="wst" id="wst" data-k="0"><span class="dot" aria-hidden="true"></span><div><b id="wstA">Đang kiểm tra…</b><small id="wstB"></small></div></div>
    <p class="wmsg" id="wMsg" role="status" aria-live="polite"></p>

    <div class="wrow">
      <h3>Mạng quanh đây</h3>
      <button type="button" class="ghost" id="wScan">Quét mạng</button>
    </div>
    <div class="nets" id="nets" aria-live="polite"></div>

    <form class="wf" id="wForm" autocomplete="off" novalidate>
      <label for="wSsid">Tên Wi-Fi</label>
      <input class="inp" id="wSsid" type="text" maxlength="32" autocomplete="off" autocapitalize="off" autocorrect="off" spellcheck="false" enterkeyhint="next">
      <label for="wPass">Mật khẩu Wi-Fi</label>
      <div class="pw">
        <input class="inp" id="wPass" type="password" maxlength="63" autocomplete="new-password" autocapitalize="off" autocorrect="off" spellcheck="false" enterkeyhint="go">
        <button type="button" class="eye" id="wEye" aria-pressed="false">Hiện</button>
      </div>
      <p class="hint">Mạng không có mật khẩu thì để trống. Chọn mạng trong danh sách để điền sẵn tên.</p>
      <button type="submit" class="go" id="wGo">Lưu và kết nối</button>
    </form>
  </div>
</div>

<script>
(function () {
  "use strict";
  var $ = function (id) { return document.getElementById(id); };
  var ZN = ["Rất khô", "Khô", "Lý tưởng", "Ướt", "Lỗi"];
  var V0 = [0, 20, 30, 50], V1 = [20, 30, 50, 100];   // cùng ngưỡng với màn TFT
  var S = null, fails = 0, busy = false, toastT = 0;

  function barsOf(r) { return r >= -60 ? 3 : (r >= -70 ? 2 : 1); }
  function put(el, txt) { if (el.textContent !== txt) el.textContent = txt; }
  function say(msg) {
    var t = $("toast"); t.textContent = msg; t.setAttribute("data-show", "1");
    clearTimeout(toastT); toastT = setTimeout(function () { t.removeAttribute("data-show"); }, 4500);
  }
  function api(path, post, body, ms) {
    var ctl = new AbortController(), to = setTimeout(function () { ctl.abort(); }, ms || 7000);
    var opt = post ? { method: "POST", headers: { "X-Req": "1" }, signal: ctl.signal } : { cache: "no-store", signal: ctl.signal };
    if (post && body != null) { opt.headers["Content-Type"] = "application/x-www-form-urlencoded"; opt.body = body; }
    return fetch(path, opt).then(function (r) {
      clearTimeout(to);
      return r.json().catch(function () { return null; }).then(function (j) { return { ok: r.ok, code: r.status, j: j }; });
    }, function (e) { clearTimeout(to); throw e; });
  }

  function render() {
    var off = fails >= 3, s = S;
    document.body.setAttribute("data-off", off ? "1" : "0");
    document.body.setAttribute("data-busy", busy ? "1" : "0");
    var link = $("link"); link.setAttribute("data-on", s && !off ? "1" : "0");
    put($("linkTxt"), off ? "Mất kết nối" : (s ? "Đã kết nối" : "Đang kết nối"));
    if (!s) return;

    var bars = barsOf(s.rssi);
    $("bars").setAttribute("data-n", off ? "0" : String(bars));
    link.title = "Wi-Fi " + s.rssi + " dBm";

    put($("temp"), s.temp == null ? "--" : s.temp.toFixed(1));
    put($("hum"), s.hum == null ? "--" : String(Math.round(s.hum)));

    var z = s.soilOk ? s.zone : 4;
    put($("soil"), s.soilOk ? String(s.soil) : "--");
    $("big").setAttribute("data-z", z);
    var zb = $("zone"); zb.setAttribute("data-z", z); put(zb, ZN[z]);
    var lis = $("zl").children;
    for (var i = 0; i < lis.length; i++) lis[i].setAttribute("data-on", s.soilOk && i === z ? "1" : "0");
    var knob = $("knob");
    if (s.soilOk) {
      var W = $("zones").clientWidth, g = 6, w = (W - 3 * g) / 4;
      var f = Math.min(1, Math.max(0, (s.soil - V0[z]) / (V1[z] - V0[z])));
      knob.style.display = "block";
      knob.style.transform = "translateX(" + (z * (w + g) + f * w).toFixed(1) + "px)";
      knob.setAttribute("data-z", z);
      knob.style.setProperty("--c", "var(--z" + z + ")");
    } else knob.style.display = "none";
    put($("soilNote"), s.soilOk ? "" : "Chưa nhận cảm biến đất. Kiểm tra dây cảm biến.");

    $("mAuto").setAttribute("aria-pressed", s.auto ? "true" : "false");
    $("mMan").setAttribute("aria-pressed", s.auto ? "false" : "true");
    $("mAuto").disabled = $("mMan").disabled = off || busy;
    put($("modeNote"), s.auto
      ? "Bơm tự bật khi độ ẩm đất dưới " + s.onBelow + "% và tắt khi từ " + s.offAt + "% trở lên." + (s.soilOk ? "" : " Chưa có số đo đất nên bơm đang tắt.")
      : "Bạn tự bật và tắt bơm. Khi chuyển sang Thủ công, bơm tự tắt.");

    document.body.setAttribute("data-pump", s.pump ? "1" : "0");
    var lk = !!s.lock;
    var ps = $("pumpState"); ps.setAttribute("data-z", lk ? "0" : (s.pump ? "3" : "4")); put(ps, lk ? "Bị khóa" : (s.pump ? "Đang bơm" : "Đã tắt"));
    put($("pumpTxt"), s.pump ? "Tắt bơm" : "Bật bơm");
    $("pumpBtn").disabled = s.auto || lk || off || busy;
    $("unlockBtn").hidden = !lk;
    $("unlockBtn").disabled = off || busy;
    put($("pumpNote"), lk ? "Bơm đã tắt và bị khóa vì chạy liên tục quá " + s.maxS + " giây. Kiểm tra bình nước rồi bấm Mở khóa bơm."
                          : (s.auto ? "Đang ở chế độ Tự động. Chuyển sang Thủ công để tự bật hoặc tắt bơm." : ""));

    var cell = $("cell");
    if (s.bat == null) { put($("batTxt"), "Không có pin"); cell.style.setProperty("--p", 0); }
    else {
      put($("batTxt"), "Pin " + s.bat + "%");
      cell.style.setProperty("--p", Math.max(0, Math.min(100, s.bat)) / 100);
      cell.setAttribute("data-lv", s.chg ? "0" : (s.bat > 30 ? "0" : (s.bat > 15 ? "1" : "2")));
    }
    put($("chgTxt"), s.chg === 1 ? "Đang sạc" : (s.chg === 2 ? "Đã đầy" : ""));
    put($("stamp"), "Cập nhật lúc " + new Date().toLocaleTimeString("vi-VN"));
  }

  function poll() {
    if (document.hidden || busy) return;
    api("/api/status").then(function (r) {
      if (r.ok && r.j) { S = r.j; fails = 0; } else fails++;
      render();
    }, function () { fails++; render(); });
  }

  function send(path) {
    if (busy) return;
    busy = true; render();
    api(path, true).then(function (r) {
      if (r.j && r.j.pump !== undefined) { S = r.j; fails = 0; }
      if (!r.ok) say(r.code === 409 ? "Đang ở chế độ Tự động nên chưa bật hoặc tắt bơm được."
                   : (r.code === 423 ? "Bơm đang bị khóa. Kiểm tra bình nước rồi bấm Mở khóa bơm." : "Thiết bị không nhận lệnh. Thử lại sau."));
    }, function () { fails++; say("Không gửi được lệnh. Kiểm tra Wi-Fi của thiết bị."); })
      .then(function () { busy = false; render(); });
  }

  $("mAuto").addEventListener("click", function () { if (S && !S.auto) send("/api/mode?auto=1"); });
  $("mMan").addEventListener("click", function () { if (S && S.auto) send("/api/mode?auto=0"); });
  $("pumpBtn").addEventListener("click", function () { if (S && !S.auto && !S.lock) send("/api/pump?on=" + (S.pump ? "0" : "1")); });
  $("unlockBtn").addEventListener("click", function () { if (S && S.lock) send("/api/pump/unlock"); });
  document.addEventListener("visibilitychange", poll);
  window.addEventListener("resize", function () { if (S) render(); });
  // ===== Đổi Wi-Fi bằng web: quét mạng -> chọn -> nhập mật khẩu -> thiết bị thử nối; nối được mới lưu, không nối được thì giữ mạng cũ =====
  var W = { open: false, scanning: false, saving: false, scanT: 0, last: null, from: null };
  var LOCK = '<svg viewBox="0 0 16 16" width="14" height="14" aria-hidden="true"><rect x="3" y="7" width="10" height="7" rx="1.6" fill="currentColor"/><path d="M5.2 7V5a2.8 2.8 0 0 1 5.6 0v2" fill="none" stroke="currentColor" stroke-width="1.6"/></svg><span class="sr">Có mật khẩu</span>';
  function blen(x) { return window.TextEncoder ? new TextEncoder().encode(x).length : unescape(encodeURIComponent(x)).length; }
  function wnote(msg, kind) {
    var m = $("wMsg"); m.textContent = msg || "";
    if (kind) m.setAttribute("data-k", kind); else m.removeAttribute("data-k");
    if (msg && !W.open) say(msg);
    if (msg && W.open) $("sheet").scrollTop = 0;                  // kết quả nằm ngay dưới ô trạng thái, luôn nhìn thấy
  }
  function paintStatus(j) {
    W.last = j;
    var a, b = "", k = "0";
    if (j.conn) { a = "Đã nối Wi-Fi “" + j.ssid + "”"; b = "Địa chỉ trang web: " + j.ip; k = "1"; }
    else if (j.trial === 1) { a = "Đang thử nối “" + j.trySsid + "”…"; b = "Chờ tối đa khoảng 25 giây."; k = "2"; }
    else { a = "Chưa nối được Wi-Fi “" + j.ssid + "”"; b = "Thiết bị vẫn tự thử nối lại. Chọn mạng khác bên dưới nếu cần."; }
    if (j.ap) b += (b ? " " : "") + "Wi-Fi riêng “" + j.apSsid + "” đang mở (" + j.apIp + ").";
    $("wst").setAttribute("data-k", k); put($("wstA"), a); put($("wstB"), b);
  }
  function wRefresh() {
    return api("/api/wifi", false, null, 5000).then(function (r) {
      if (r.ok && r.j && r.j.ssid !== undefined) { paintStatus(r.j); return r.j; }
      throw 0;
    }).catch(function () {
      if (!W.last) { $("wst").setAttribute("data-k", "0"); put($("wstA"), "Chưa đọc được Wi-Fi của thiết bị"); put($("wstB"), "Kiểm tra điện thoại còn nối Wi-Fi của thiết bị."); }
      return null;
    });
  }

  function wOpen(skipRefresh) {
    if (W.open) return;
    W.open = true; W.from = document.activeElement;
    $("ov").setAttribute("data-open", "1"); document.documentElement.setAttribute("data-lock", "1");
    if (!W.saving) wnote("");
    $("wClose").focus();
    if (!skipRefresh) wRefresh();
    wScan();
  }
  function wClose() {
    if (!W.open) return;
    W.open = false; clearTimeout(W.scanT); W.scanning = false; scanUi(false);
    $("ov").setAttribute("data-open", "0"); document.documentElement.removeAttribute("data-lock");
    if (W.from && W.from.focus) W.from.focus();
  }

  function scanUi(on) { var b = $("wScan"); b.disabled = on; put(b, on ? "Đang quét…" : "Quét lại"); }
  function netsMsg(t) { var box = $("nets"); box.textContent = ""; var p = document.createElement("p"); p.textContent = t; box.appendChild(p); }
  function scanEnd(msg) { W.scanning = false; scanUi(false); if (msg) netsMsg(msg); }
  function wScan() {
    if (W.scanning || !W.open) return;
    W.scanning = true; scanUi(true); netsMsg("Đang tìm các mạng Wi-Fi quanh đây…");
    api("/api/wifi/scan?new=1", false, null, 6000).then(function (r) {
      if (!W.open) return scanEnd();
      if (!r.ok || !r.j) return scanEnd("Không quét được. Bấm Quét lại để thử lần nữa.");
      if (r.j.scanning) scanNext(0, 0); else showNets(r.j);
    }, function () { scanEnd("Không quét được. Kiểm tra điện thoại còn nối Wi-Fi của thiết bị."); });
  }
  function scanNext(n, bad) {
    W.scanT = setTimeout(function () {
      if (!W.open) return scanEnd();
      var again = function (b) { if (b > 4 || n > 20) scanEnd("Quét quá lâu. Bấm Quét lại để thử lần nữa."); else scanNext(n + 1, b); };
      api("/api/wifi/scan", false, null, 4000).then(function (r) {
        if (!W.open) return scanEnd();
        if (r.ok && r.j) { if (r.j.scanning) again(0); else showNets(r.j); } else again(bad + 1);
      }, function () { again(bad + 1); });
    }, 1400);
  }
  function showNets(j) {
    scanEnd();
    var list = j.nets || [], box = $("nets");
    if (!list.length) { netsMsg(j.err ? "Không quét được. Bấm Quét lại để thử lần nữa." : "Không thấy mạng nào. Đứng gần router hơn rồi quét lại, hoặc gõ tên Wi-Fi vào ô bên dưới."); return; }
    box.textContent = "";
    list.forEach(function (n) { box.appendChild(netRow(n)); });
  }
  function netRow(n) {
    var b = document.createElement("button"); b.type = "button"; b.className = "net";
    b.setAttribute("aria-pressed", $("wSsid").value === n.s ? "true" : "false");
    var nm = document.createElement("span"); nm.className = "nm"; nm.textContent = n.s; b.appendChild(nm);     // tên mạng do người lạ đặt: chỉ đưa vào bằng textContent
    if (W.last && W.last.conn && W.last.ssid === n.s) { var t = document.createElement("span"); t.className = "tag"; t.textContent = "Đang dùng"; b.appendChild(t); }
    var lk = document.createElement("span"); lk.className = "lk"; lk.innerHTML = n.l ? LOCK : ""; b.appendChild(lk);
    var br = document.createElement("span"); br.className = "bars"; br.setAttribute("data-n", String(barsOf(n.r))); br.innerHTML = "<i></i><i></i><i></i>"; b.appendChild(br);
    b.addEventListener("click", function () {
      $("wSsid").value = n.s; $("wPass").value = ""; markNets();
      (n.l ? $("wPass") : $("wGo")).focus();
    });
    return b;
  }
  function markNets() {
    var v = $("wSsid").value, rows = $("nets").querySelectorAll(".net");
    for (var i = 0; i < rows.length; i++) rows[i].setAttribute("aria-pressed", rows[i].firstChild.textContent === v ? "true" : "false");
  }

  function saveUi(on) { W.saving = on; var g = $("wGo"); g.disabled = on; put(g, on ? "Đang nối…" : "Lưu và kết nối"); }
  function wSave(ev) {
    ev.preventDefault();
    if (W.saving) return;
    var s = $("wSsid").value, p = $("wPass").value;
    if (!s) { wnote("Chưa có tên Wi-Fi. Chọn một mạng trong danh sách hoặc gõ tên vào ô Tên Wi-Fi.", "err"); $("wSsid").focus(); return; }
    if (blen(s) > 32) { wnote("Tên Wi-Fi dài quá 32 byte nên thiết bị không nhận được.", "err"); $("wSsid").focus(); return; }
    if (p && (blen(p) < 8 || blen(p) > 63)) { wnote("Mật khẩu Wi-Fi phải dài từ 8 đến 63 ký tự. Mạng không có mật khẩu thì để trống.", "err"); $("wPass").focus(); return; }
    saveUi(true); wnote("Đang gửi cho thiết bị…", "info");
    api("/api/wifi/save", true, "ssid=" + encodeURIComponent(s) + "&pass=" + encodeURIComponent(p), 9000).then(function (r) {
      if (!r.ok) {
        saveUi(false);
        wnote(r.code === 400 ? "Thiết bị không nhận thông tin này. Kiểm tra lại tên và mật khẩu." : "Thiết bị không nhận lệnh (mã " + r.code + "). Thử lại sau.", "err");
        return;
      }
      if (r.j && r.j.ssid !== undefined) paintStatus(r.j);
      wnote("Đang nối “" + s + "”. Chờ khoảng 25 giây. Nếu điện thoại bị rớt Wi-Fi của thiết bị trong lúc này thì cứ để yên, trang sẽ tự cập nhật khi nối lại được.", "info");
      watch(s, Date.now(), 0);
    }, function () {
      saveUi(false);
      wnote("Không gửi được cho thiết bị. Kiểm tra điện thoại còn nối Wi-Fi của thiết bị rồi thử lại.", "err");
      wRefresh();
    });
  }
  function watch(s, t0, bad) {
    setTimeout(function () {
      var next = function (b) {
        if (Date.now() - t0 > 80000) return wDone("lost", null, s);
        if (b === 3) wnote("Đang mất liên lạc với thiết bị. Bình thường khi thiết bị đổi mạng, đang chờ nối lại.", "info");
        watch(s, t0, b);
      };
      api("/api/wifi", false, null, 3500).then(function (r) {
        if (r.ok && r.j && r.j.ssid !== undefined) {
          paintStatus(r.j);
          if (r.j.trial === 2) return wDone("ok", r.j, s);
          if (r.j.trial === 3) return wDone("fail", r.j, s);
          next(0);
        } else next(bad + 1);
      }, function () { next(bad + 1); });
    }, 1500);
  }
  function wDone(kind, j, s) {
    saveUi(false);
    if (kind === "ok") {
      var via = /^192\.168\.4\./.test(location.hostname);
      wnote("Đã nối “" + s + "” và lưu lại. Từ giờ thiết bị tự dùng mạng này. Địa chỉ trang web: http://" + j.ip + " hoặc http://" + (j.host || "tuoicay") + ".local" +
        (via ? ". Trang này đang mở qua Wi-Fi riêng của thiết bị (sẽ tự tắt sau ít phút). Hãy nối điện thoại về cùng Wi-Fi “" + s + "” rồi mở địa chỉ trên." : "."), "ok");
    } else if (kind === "fail") {
      wnote("Không nối được “" + s + "”. Có thể sai mật khẩu hoặc sóng quá yếu. Thiết bị chưa lưu gì và đang nối lại Wi-Fi cũ “" + j.ssid + "”.", "err");
    } else {
      wnote("Chưa nhận được kết quả từ thiết bị. Nếu điện thoại đã bị chuyển sang Wi-Fi khác, hãy nối lại Wi-Fi của thiết bị hoặc Wi-Fi nhà rồi mở lại trang này để xem thiết bị đang nối mạng nào.", "err");
    }
  }

  $("wBtn").addEventListener("click", function () { wOpen(false); });
  $("wClose").addEventListener("click", wClose);
  $("ov").addEventListener("click", function (e) { if (e.target === $("ov")) wClose(); });
  $("wScan").addEventListener("click", wScan);
  $("wForm").addEventListener("submit", wSave);
  $("wSsid").addEventListener("input", markNets);
  $("wEye").addEventListener("click", function () {
    var show = $("wPass").type === "password";
    $("wPass").type = show ? "text" : "password";
    $("wEye").setAttribute("aria-pressed", show ? "true" : "false"); put($("wEye"), show ? "Ẩn" : "Hiện");
  });
  document.addEventListener("keydown", function (e) {
    if (!W.open) return;
    if (e.key === "Escape") { wClose(); return; }
    if (e.key !== "Tab") return;                                   // giữ phím Tab trong khung
    var f = $("sheet").querySelectorAll("button:not(:disabled),input"), first = f[0], last = f[f.length - 1];
    if (e.shiftKey && document.activeElement === first) { e.preventDefault(); last.focus(); }
    else if (!e.shiftKey && document.activeElement === last) { e.preventDefault(); first.focus(); }
  });
  // thiết bị đang mở Wi-Fi riêng vì chưa nối được mạng nhà (và đây là lần mở trang đầu): hiện luôn khung chọn mạng
  wRefresh().then(function (j) { if (j && j.ap && !j.conn) wOpen(true); });
  setInterval(poll, 1000);
  poll();
})();
</script>
</body>
</html>
)HTML";
