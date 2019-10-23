from flask import jsonify, render_template


def return_api(success, data=None, error=None):
    return jsonify(
        {
            "success": str(success),
            "data": str(data),
            "error": str(error)
        }
    )


def return_front_message(title, content, r_time, r_url):
    return render_template('message.html',
                           title=title,
                           content=content,
                           r_time=str(r_time),
                           r_url=r_url)
