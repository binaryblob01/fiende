; AisleRiot - block_ten.scm
; Copyright (C) 2001 Rosanna Yuen <zana@webwynk.net>
;
; This game is free software; you can redistribute it and/or modify
; it under the terms of the GNU General Public License as published by
; the Free Software Foundation; either version 2, or (at your option)
; any later version.
;
; This program is distributed in the hope that it will be useful,
; but WITHOUT ANY WARRANTY; without even the implied warranty of
; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
; GNU General Public License for more details.
;
; You should have received a copy of the GNU General Public License
; along with this program; if not, write to the Free Software
; Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307
; USA

(define (new-game)
  (initialize-playing-area)
  (make-standard-deck)
  (shuffle-deck)

  (add-normal-slot DECK)
  (add-blank-slot)

  (add-normal-slot '())
  (add-normal-slot '())
  (add-normal-slot '())
  (add-carriage-return-slot)

  (add-blank-slot)
  (add-blank-slot)
  (add-normal-slot '())
  (add-normal-slot '())
  (add-normal-slot '())
  (add-carriage-return-slot)

  (add-blank-slot)
  (add-blank-slot)
  (add-normal-slot '())
  (add-normal-slot '())
  (add-normal-slot '())

  (deal-cards-face-up 0 '(1 2 3 4 5 6 7 8 9))
  (give-status-message)

  (list 5 3))

(define (give-status-message)
  (set-statusbar-message (get-stock-no-string)))

(define (get-stock-no-string)
  (string-append "Stock left:  " 
		 (number->string (length (get-cards 0)))))

(define (button-pressed slot-id card-list)
  (and (not (empty-slot? slot-id))
       (not (= 0 slot-id))))

(define (check-for-deal start-slot end-slot)
  (or (empty-slot? 0)
      (and (deal-cards-face-up 0 (list start-slot))
	   (or (empty-slot? 0)
	       (deal-cards-face-up 0 (list end-slot))))))

(define (button-released start-slot card-list end-slot)
  (and (not (= end-slot 0))
       (not (empty-slot? end-slot))
       (or (= 10 (+ (get-value (car card-list))
		    (get-value (get-top-card end-slot))))
	   (and (> (get-value (car card-list)) 10)
		(= (get-value (get-top-card end-slot))
		   (get-value (car card-list)))))
       (remove-card end-slot)
       (check-for-deal start-slot end-slot)
       (add-to-score! 2)))

(define (button-clicked slot-id)
  #f)

(define (button-double-clicked slot-id)
  #f)

(define (game-continuable)
  (give-status-message)
  (and (not (game-won))
       (get-hint)))

(define (game-won)
  (and (or (empty-slot? 1)
	   (= (get-value (get-top-card 1)) 10))
       (or (empty-slot? 2)
	   (= (get-value (get-top-card 2)) 10))
       (or (empty-slot? 3)
	   (= (get-value (get-top-card 3)) 10))
       (or (empty-slot? 4)
	   (= (get-value (get-top-card 4)) 10))
       (or (empty-slot? 5)
	   (= (get-value (get-top-card 5)) 10))
       (or (empty-slot? 6)
	   (= (get-value (get-top-card 6)) 10))
       (or (empty-slot? 7)
	   (= (get-value (get-top-card 7)) 10))
       (or (empty-slot? 8)
	   (= (get-value (get-top-card 8)) 10))
       (or (empty-slot? 9)
	   (= (get-value (get-top-card 9)) 10))))

(define (avail-pair? slot1 slot2)
  (cond ((= slot1 9)
	 #f)
	((or (empty-slot? slot1)
	     (= slot2 10))
	 (avail-pair? (+ 1 slot1) (+ 2 slot1)))
	((and (not (empty-slot? slot2))
	      (or (= 10 (+ (get-value (get-top-card slot1))
			   (get-value (get-top-card slot2))))
		  (and (> (get-value (get-top-card slot1)) 10)
		       (= (get-value (get-top-card slot1))
			  (get-value (get-top-card slot2))))))
	 (list 1 
	       (get-name (get-top-card slot1)) 
	       (get-name (get-top-card slot2))))
	(#t (avail-pair? slot1 (+ 1 slot2)))))

(define (get-hint)
  (avail-pair? 1 2))

(define (get-options) 
  #f)

(define (apply-options options) 
  #f)

(define (timeout) 
  #f)

(set-lambda new-game button-pressed button-released button-clicked
button-double-clicked game-continuable game-won get-hint get-options
apply-options timeout)
