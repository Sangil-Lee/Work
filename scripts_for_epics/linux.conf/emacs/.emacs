; menu bar off
;(menu-bar-mode nil)

; tool bar off
;(tool-bar-mode nil)

(setq column-number-mode t)
; open .gz file
(auto-compression-mode t)

;; Paren highlighting
(require 'paren)
(show-paren-mode 1)

;; default to better frame titles
(setq frame-title-format (concat invocation-name "@" system-name ": %b %+%+ %f"))

(transient-mark-mode 1)

;; see how to format
;; http://www.delorie.com/gnu/docs/elisp-manual-21/elisp_689.html
;; %A : This stands for the full name of the day of week. 
;; %B : This stands for the full name of the month. 
;; %e : This stands for the day of month, blank-padded. 
;; %H : his stands for the hour (00-23). 
;; %M : This stands for the minute (00-59). 
;; %S : This stands for the seconds (00-59). 
;; %Z : This stands for the time zone abbreviation. 
;; %Y : This stands for the year with century. 

(defvar insert-date-format "%A, %B %e %H:%M:%S %Z %Y")

(defun insert-date ()
  "Insert the current date"
  (interactive "*")
  (insert (format-time-string insert-date-format (current-time) ))
)


 
(global-set-key [?\S- ] 'toggle-input-method)
(global-set-key [f5] 'insert-date)

;; not necessary for 23-2 emacs
;;(global-set-key (kbd "C-x <left>") 'previous-buffer)
;;(global-set-key (kbd "C-x <right>") 'next-buffer)


;; handle color codes properly in shell enviroment into emacs shell

(add-hook 'shell-mode-hook 'ansi-color-for-comint-mode-on)

;; (when window-system
;;    (set-face-font 'default "-outline-Droid Sans Mono-normal-normal-normal-mono-14-*-*-*-c-*-iso10646-1")
;;     (set-fontset-font "fontset-default" '(#x1100 . #xffdc)  '("나눔고딕코딩" . "unicode-bmp")) ;; 유니코드 한글영역
;;     (set-fontset-font "fontset-default" '(#xe0bc . #xf66e)  '("나눔굴림" . "unicode-bmp")) ;; 유니코드 사용자 영역
;; )


(load-file "$HOME/.emacs.d/epics-mode.el")
(require 'epics-mode)



(autoload 'markdown-mode "markdown-mode"
   "Major mode for editing Markdown files" t)
(add-to-list 'auto-mode-alist '("\\.text\\'" . markdown-mode))
(add-to-list 'auto-mode-alist '("\\.markdown\\'" . markdown-mode))
(add-to-list 'auto-mode-alist '("\\.md\\'" . markdown-mode))


(custom-set-variables
  ;; custom-set-variables was added by Custom.
  ;; If you edit it by hand, you could mess it up, so be careful.
  ;; Your init file should contain only one such instance.
  ;; If there is more than one, they won't work right.
 '(column-number-mode t)
 '(doc-view-continuous t)
  '(font-use-system-font t)
 '(inhibit-startup-screen t)
 '(ispell-personal-dictionary "~/.aspell.LANG.pws")
 '(ispell-skip-html t)
 '(safe-local-variable-values (quote ((TeX-master . t))))
 '(show-paren-mode t)
 '(text-mode-hook (quote (text-mode-hook-identify))))
(custom-set-faces
  ;; custom-set-faces was added by Custom.
  ;; If you edit it by hand, you could mess it up, so be careful.
  ;; Your init file should contain only one such instance.
  ;; If there is more than one, they won't work right.
 '(default ((t (:inherit nil :stipple nil :background "white" :foreground "black" :inverse-video nil :box nil :strike-through nil :overline nil :underline nil :slant normal :weight normal :height 99 :width normal :foundry "unknown" :family "DejaVu Sans Mono")))))
